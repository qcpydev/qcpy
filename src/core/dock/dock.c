#include <assert.h>
#include <base.h>
#include <base_tools.h>
#include <dock.h>
#include <dock_logger.h>
#include <fcntl.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

export_t *exporter = NULL;
import_t *importer = NULL;

sem_t *dock_import_sem;
sem_t *port_import_sem;

sem_t *dock_export_sem;
sem_t *port_export_sem;

int shared_export_space;

block_buffer_global_t *block_buffer_global_queues = NULL;
int global_queues_shared_fd = -1;

dock_thread_pool_t dock_thread_pool;

void dock_wait_for_boot() {
  atomic_size_t total = 0;
  do {
    total = atomic_load_explicit(&block_buffer_global_queues->total_queues,
                                 memory_order_relaxed);
  } while (total + 1 != IMPORTER_FUNNEL);
}

static void dock_create_importer() {
  importer =
      (import_t *)base_tools_create_shared_mem(sizeof(import_t), QCPY_IMPORT);
  assert(importer);

  port_import_sem = base_tools_create_shared_sem(PORT_IMPORT_SEM, 0);
  assert(port_import_sem);

  dock_import_sem = base_tools_create_shared_sem(DOCK_IMPORT_SEM, 0);
  assert(dock_import_sem);
}

static void dock_create_exporter() {
  assert(!exporter);
  exporter =
      (export_t *)base_tools_create_shared_mem(sizeof(export_t), QCPY_EXPORT);
  assert(exporter);

  port_export_sem = base_tools_create_shared_sem(PORT_EXPORT_SEM, 0);
  assert(port_export_sem);

  dock_export_sem = base_tools_create_shared_sem(DOCK_EXPORT_SEM, 0);
  assert(dock_export_sem);
}

void dock_port_init() {
  assert(!block_buffer_global_queues);
  global_queues_shared_fd = base_tools_create_shared_mem_new(
      sizeof(block_buffer_global_t), BLOCK_BUFFER_QUEUE_GLOBAL_NAME,
      (void **)&block_buffer_global_queues);

  assert(block_buffer_global_queues);
  assert(global_queues_shared_fd != -1);

  // dock_create_importer();
  // dock_create_exporter();

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
    char shared_mem_fd[20];
    snprintf(shared_mem_fd, sizeof(shared_mem_fd), "%d",
             global_queues_shared_fd);
    char *args_test[] = {boot_thread_args->exec_name, shared_mem_fd, NULL};
    execvp(boot_thread_args->exec_name, args_test);
    perror("execvp failed");
    _exit(1);
  }

  pid_t quack_pid = 1;
  if (quack_pid == 0) {
    prctl(PR_SET_PDEATHSIG, SIGTERM);
    char *args_test[] = {boot_thread_args->quack_exec_name, NULL};

    char *run_quack_version = boot_thread_args->is_gpu_enabled
                                  ? boot_thread_args->quack_gpu_exec_name
                                  : boot_thread_args->quack_exec_name;

    execvp(run_quack_version, args_test);
    perror("execvp failed");
    _exit(1);
  }
}

static void dock_flush_entries() {
  sem_post(dock_import_sem);
  sem_wait(port_import_sem);

  importer->flushing = false;
  importer->flush_reg = 0;

  assert(importer->idx == 0);
}

void dock_get_qc_state(int flush_reg) {}

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
  dock_enqueue(block);
  return 1;
}

void dock_queue_init(dock_queue_t *dock_queue) {
  assert(dock_queue);
  pthread_mutex_init(&dock_queue->lock, NULL);
  pthread_cond_init(&dock_queue->cond, NULL);
}

block_t *dock_dequeue(dock_queue_t *dock_queue) {
  assert(dock_queue);
  block_t *block = NULL;

  pthread_mutex_lock(&dock_queue->lock);

  while (dock_queue->item_count == 0) {
    pthread_cond_wait(&dock_queue->cond, &dock_queue->lock);
  }

  dock_queue_item_t *head = dock_queue->head;

  assert(head);

  block = head->block;

  dock_queue->head = head->next;

  if (!dock_queue->head) {
    dock_queue->tail = NULL;
  }

  assert(dock_queue->item_count > 0);
  --dock_queue->item_count;

  pthread_mutex_unlock(&dock_queue->lock);

  free(head);

  return block;
}

static void *dock_queue_dequeue_worker_thread(void *queue_id) {
  assert(&queue_id);
  size_t id = (size_t)queue_id;
  assert(id < DOCK_QUEUES_TOTAL && id >= 0);
  dock_queue_t *dock_queue = &dock_thread_pool.queue[id];

  for (;;) {
    block_t *block = dock_dequeue(dock_queue);
    block_enqueue(block_buffer_global_queues, block);
  }
}

void *dock_thread_pool_init(void *not_used) {
  (void)not_used;

  for (size_t i = 0; i < DOCK_QUEUES_TOTAL; ++i) {
    dock_queue_init(&dock_thread_pool.queue[i]);
    pthread_create(&dock_thread_pool.threads[i], NULL,
                   dock_queue_dequeue_worker_thread, (void *)i);
  }

  for (size_t i = 0; i < DOCK_QUEUES_TOTAL; ++i) {
    pthread_join(dock_thread_pool.threads[i], NULL);
  }

  return NULL;
}
