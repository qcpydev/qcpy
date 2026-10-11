#include <assert.h>
#include <base.h>
#include <base_tools.h>
#include <dock.h>
#include <dock_logger.h>
#include <exporter_migrate.h>
#include <fcntl.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

export_t *exporter;
int shared_export_space = -1;

block_buffer_global_t *block_buffer_global_queues = NULL;
int global_queues_shared_fd = -1;

int temp_migrate_memfd;
exporter_migrate_t *exporter_migrate_watcher;

dock_thread_pool_t dock_thread_pool;

void dock_wait_for_qcpy_core_boot() {
  atomic_size_t total = 0;
  atomic_bool export_ready = false;
  atomic_bool exporter_migrate_ready = false;

  do {
    total = atomic_load_explicit(&block_buffer_global_queues->total_queues,
                                 memory_order_acquire);
    export_ready =
        atomic_load_explicit(&exporter->export_ready, memory_order_acquire);
  } while (total + 1 != DOCK_QUEUES_TOTAL && export_ready == false);
}

void dock_port_init() {
  assert(!exporter);
  shared_export_space = base_tools_create_shared_mem(
      sizeof(export_t), QCPY_EXPORT, (void **)&exporter);
  assert(exporter && shared_export_space != -1);

  assert(!block_buffer_global_queues);
  global_queues_shared_fd = base_tools_create_shared_mem(
      sizeof(block_buffer_global_t), BLOCK_BUFFER_QUEUE_GLOBAL_NAME,
      (void **)&block_buffer_global_queues);

  temp_migrate_memfd = base_tools_create_shared_mem(
      sizeof(exporter_migrate_t), EXPORTER_MIGRATE_SHARED_MEM,
      (void **)&exporter_migrate_watcher);

  assert(block_buffer_global_queues);
  assert(global_queues_shared_fd != -1);

  dock_thread_pool_launch();
}

char *dock_init(char *args[]) {
  if (!args) {
    return NULL;
  }

  return args[0];
}

char *dock_quack_init(char *args[]) {
  if (!args) {
    return NULL;
  }

  return args[1];
}

char *dock_quack_gpu_init(char *args[]) {
  if (!args) {
    return NULL;
  }
  return args[2];
}

bool dock_set_gpu_enabled(char *args[]) { return (!strcmp(args[3], "Y")); }

void dock_run_boot() {
  assert(boot_thread_args);
  dock_port_init();

  pid_t pid = fork();
  if (pid == 0) {
    prctl(PR_SET_PDEATHSIG, SIGTERM);
    char buffer_queues_memfd_id[20];
    snprintf(buffer_queues_memfd_id, sizeof(buffer_queues_memfd_id), "%d",
             global_queues_shared_fd);

    char exporter_memfd_id[20];
    snprintf(exporter_memfd_id, sizeof(exporter_memfd_id), "%d",
             shared_export_space);

    char exporter_migrate_memfd[20];
    snprintf(exporter_migrate_memfd, sizeof(exporter_migrate_memfd), "%d",
             temp_migrate_memfd);

    char *args_test[] = {boot_thread_args->exec_name, buffer_queues_memfd_id,
                         exporter_memfd_id, exporter_migrate_memfd, NULL};
    execvp(boot_thread_args->exec_name, args_test);
    perror("execvp failed");
    _exit(1);
  }

  dock_wait_for_qcpy_core_boot();

  pid_t quack_pid = fork();
  if (quack_pid == 0) {
    prctl(PR_SET_PDEATHSIG, SIGTERM);

    char exporter_migrate_memfd[20];
    snprintf(exporter_migrate_memfd, sizeof(exporter_migrate_memfd), "%d",
             temp_migrate_memfd);

    char *args_test[] = {boot_thread_args->quack_exec_name,
                         exporter_migrate_memfd, NULL};

    char *run_quack_version = boot_thread_args->is_gpu_enabled
                                  ? boot_thread_args->quack_gpu_exec_name
                                  : boot_thread_args->quack_exec_name;

    execvp(run_quack_version, args_test);
    perror("execvp failed");
    _exit(1);
  }
}

void dock_get_qc_state(uint16_t flush_reg) {
  block_t *temp_block = (block_t *)malloc(sizeof(block_t));
  assert(temp_block);
  memset(temp_block, 0, sizeof(block_t));
  temp_block->reg = flush_reg;
  temp_block->type = BLOCK_FLUSH_ENTRY;

  block_enqueue(block_buffer_global_queues, temp_block);

  usleep(1000);
}

void dock_thread_pool_launch() {
  pthread_create(&dock_thread_pool.boot_thread, NULL, dock_thread_pool_init,
                 NULL);
}

static struct dock_queue_item_t *dock_queue_item_init(block_t *block) {
  struct dock_queue_item_t *dock_queue_item =
      (struct dock_queue_item_t *)malloc(sizeof(struct dock_queue_item_t));

  dock_queue_item->block = block;
  dock_queue_item->next = NULL;

  return dock_queue_item;
}

void dock_enqueue(block_t *block) {
  assert(0);
  struct dock_queue_item_t *dock_queue_item = dock_queue_item_init(block);
  assert(&dock_queue_item);

  size_t idx = block->reg % DOCK_QUEUES_TOTAL;
  dock_queue_t *dock_queue = &dock_thread_pool.queue[idx];

  pthread_mutex_lock(&dock_queue->lock);

  if (!dock_queue->head) {
    dock_queue->head = dock_queue_item;
    dock_queue->tail = dock_queue_item;
  } else {
    dock_queue->tail->next = dock_queue_item;
    dock_queue->tail = dock_queue->tail->next;
  }

  ++dock_queue->item_count;

  pthread_cond_signal(&dock_queue->cond);
  pthread_mutex_unlock(&dock_queue->lock);
}

int dock_add(block_t *block) {
  block_enqueue(block_buffer_global_queues, block);
  return 1;
}

static void *dock_queue_dequeue_worker_thread(void *queue_id) {
  assert(&queue_id);
  size_t id = (size_t)queue_id;
  assert(id < DOCK_QUEUES_TOTAL && id >= 0);
  dock_queue_t *dock_queue = &dock_thread_pool.queue[id];

  pthread_mutex_lock(&dock_queue->lock);
  for (;;) {
    assert(dock_queue);
    block_t *block = NULL;

    while (dock_queue->item_count == 0) {
      pthread_cond_wait(&dock_queue->cond, &dock_queue->lock);
    }

    assert(dock_queue->item_count != 0);

    dock_queue_item_t *head = dock_queue->head;

    if (head) {
      block = head->block;

      dock_queue->head = head->next;

      if (!dock_queue->head) {
        dock_queue->tail = NULL;
      }

      assert(dock_queue->item_count > 0);
      --dock_queue->item_count;
      block_enqueue(block_buffer_global_queues, block);
      free(head);
    }
  }

  pthread_mutex_unlock(&dock_queue->lock);
}

void *dock_thread_pool_init(void *not_used) {
  (void)not_used;

  return NULL;
}
