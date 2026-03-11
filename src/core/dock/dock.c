#include <assert.h>
#include <dock.h>
#include <fcntl.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

import_t *importer = NULL;
export_t *exporter = NULL;

sem_t *dock_import_sem;
sem_t *port_import_sem;

sem_t *dock_export_sem;
sem_t *port_export_sem;

int shared_import_space;
int shared_export_space;

export_signal_t *export_signal;
sem_t *export_sig_stop;
sem_t *export_sig_ready;
int shared_export_signal_space;

dock_logger_t dock_log;

void dock_wait_for_boot() {
  while (!importer || !exporter || !importer->ready ||
         !exporter->qcpy_core_ready || !export_signal->qcpy_core_ready)
    ;
}

static void dock_create_importer() {
  assert(!importer);
  shared_import_space =
      shm_open(QCPY_IMPORT, OFLAG_SHARED_MEM_ARGS, MODE_SHARED_MEM);
  if (shared_import_space < 0) {
    perror("shm_open");
    _exit(1);
    assert(0);
  }

  if (ftruncate(shared_import_space, sizeof(import_t)) < 0) {
    perror("ftruncate");
    assert(0);
  }

  importer = mmap(NULL, sizeof(*importer), PROT_ARGS, MAP_SHARED,
                  shared_import_space, 0);

  if (importer == MAP_FAILED) {
    perror("map_failed");
    assert(0);
  }

  memset(importer, 0, sizeof(import_t));

  sem_unlink(DOCK_IMPORT_SEM);
  dock_import_sem =
      sem_open(DOCK_IMPORT_SEM, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

  sem_unlink(PORT_IMPORT_SEM);
  port_import_sem =
      sem_open(PORT_IMPORT_SEM, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

  if (!dock_import_sem) {
    perror("import_sem_failed");
    assert(0);
  }

  if (!port_import_sem) {
    perror("import_sem_failed");
    assert(0);
  }
}

static void dock_create_exporter_signal() {
  assert(!export_signal);

  shared_export_signal_space = shm_open(EXPORT_SIGNAL_SHARED_MEM,
                                        OFLAG_SHARED_MEM_ARGS, MODE_SHARED_MEM);

  if (shared_export_signal_space < 0) {
    perror("shm_open");
    _exit(1);
    assert(0);
  }

  if (ftruncate(shared_export_signal_space, sizeof(export_signal_t)) < 0) {
    perror("ftruncate");
    assert(0);
  }

  export_signal = mmap(NULL, sizeof(*export_signal), PROT_ARGS, MAP_SHARED,
                       shared_export_signal_space, 0);

  if (export_signal == MAP_FAILED) {
    perror("map_failed");
    assert(0);
  }
  memset(export_signal, 0, sizeof(export_t));

  sem_unlink(EXPORT_SIGNAL_STOP);
  export_sig_stop =
      sem_open(EXPORT_SIGNAL_STOP, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 1);

  if (!export_sig_stop) {
    perror("export_signal_failed");
    assert(0);
  }

  sem_unlink(EXPORT_SIGNAL_READY);
  export_sig_ready =
      sem_open(EXPORT_SIGNAL_READY, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

  if (!export_sig_ready) {
    perror("export_signal_failed");
    assert(0);
  }
}

static void dock_create_exporter() {
  assert(!exporter);
  shared_export_space =
      shm_open(QCPY_EXPORT, OFLAG_SHARED_MEM_ARGS, MODE_SHARED_MEM);
  if (shared_export_space < 0) {
    perror("shm_open");
    _exit(1);
    assert(0);
  }

  if (ftruncate(shared_export_space, sizeof(export_t)) < 0) {
    perror("ftruncate");
    assert(0);
  }

  exporter = mmap(NULL, sizeof(*exporter), PROT_ARGS, MAP_SHARED,
                  shared_export_space, 0);

  if (exporter == MAP_FAILED) {
    perror("map_failed");
    assert(0);
  }

  memset(exporter, 0, sizeof(export_t));
  sem_unlink(DOCK_EXPORT_SEM);
  dock_export_sem = sem_open(DOCK_EXPORT_SEM, OFLAG_SHARED_SEM_ARGS,
                             MODE_SHARED_MEM, IMPORT_MAX_SIZE);

  sem_unlink(PORT_EXPORT_SEM);
  port_export_sem =
      sem_open(PORT_EXPORT_SEM, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

  if (!dock_export_sem) {
    perror("export_sem_failed");
    assert(0);
  }

  if (!port_export_sem) {
    perror("export_sem_failed");
    assert(0);
  }

  dock_create_exporter_signal();
}

void dock_port_init() {
  dock_create_exporter();
  dock_create_importer();
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
    char *args_test[] = {boot_thread_args->exec_name, NULL};
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

static void dock_signal_export(uint64_t reg) {
  assert(export_signal);

  sem_wait(export_sig_stop);
  export_signal->reg = reg;
  sem_post(export_sig_ready);
}

static bool dock_await_export_entries() { return false; }

static void dock_flush_entries() {
  sem_post(dock_import_sem);

  sem_wait(port_import_sem);
  assert(importer->idx == 0);

  importer->flushing = false;
  importer->flush_reg = 0;
}

int dock_add(block_t *block) {
  dock_log_append(block->reg, block);

  block_add(block, importer);

  if (importer->idx == IMPORT_MAX_SIZE) {
    dock_flush_entries();
  }

  return 1;
}

void dock_get_qc_state(int flush_reg) {
  assert(importer);

  importer->flushing = true;
  importer->flush_reg = flush_reg;

  if (importer->idx) {
    dock_flush_entries();
  }

  dock_signal_export(flush_reg);

  /*
   * TODO: await for exporter to let us know it is ready for us to consume data,
   * use a do while loop to await for entries to be calculated
   */
}

int dock_get_qc_entries(uint32_t reg, block_t *blocks) {
  dock_log_t *dock_logger = dock_log_find(reg);
  return dock_log_fill_array(dock_logger, blocks);
}

dock_log_t *dock_log_init(uint32_t reg, block_t *block) {
  assert(block);

  dock_log_t *new_dock_log = (dock_log_t *)malloc(sizeof(dock_log_t));
  memset(new_dock_log, 0, sizeof(dock_log_t));

  new_dock_log->reg = reg;

  if (!dock_log.logs) {
    dock_log.logs = new_dock_log;
    dock_log.last = new_dock_log;
  } else {
    assert(dock_log.logs);
    dock_log.last->next = new_dock_log;
    dock_log.last = dock_log.last->next;
  }

  dock_log.last->queue = dock_log_item_init(block);
  assert(dock_log.last->queue);
  return new_dock_log;
}

dock_log_item_t *dock_log_item_init(block_t *block) {
  assert(block);

  dock_log_item_t *dock_log_item = NULL;

  dock_log_item = (dock_log_item_t *)malloc(sizeof(dock_log_item_t));
  memset(dock_log_item, 0, sizeof(dock_log_item_t));
  memcpy(&dock_log_item->block, block, sizeof(block_t));

  return dock_log_item;
}

dock_log_t *dock_log_find(uint32_t reg) {
  if (dock_log.last_used && dock_log.last_used->reg == reg) {
    return dock_log.last_used;
  }

  dock_log_t *dock_log_walker = dock_log.logs->next;

  while (dock_log_walker && dock_log_walker->reg != reg) {
    dock_log_walker = dock_log_walker->next;
  }

  if (dock_log_walker) {
    dock_log.last_used = dock_log_walker;
  }

  return dock_log_walker;
}

void dock_log_append(uint32_t reg, block_t *block) {
  assert(block);
  dock_log_t *to_append = dock_log.logs;

  if (!to_append) {
    to_append = dock_log_init(reg, block);
    dock_log.last_used = to_append;
    return;
  }

  to_append = dock_log_find(reg);

  if (!to_append) {
    to_append = dock_log_init(reg, block);
    dock_log.last_used = to_append;
    return;
  }

  assert(to_append && to_append->queue);

  dock_log_item_t *item = dock_log_item_init(block);

  if (!to_append->queue->next) {
    to_append->queue->next = item;
    to_append->last = item;
  } else {
    to_append->last->next = item;
    to_append->last = to_append->last->next;
  }
}

int dock_log_fill_array(dock_log_t *log, block_t *blocks) {
  assert(log && blocks);
  int count = 0;

  dock_log_item_t *queue = log->queue;
  assert(queue);

  if (log->checkpoint) {
    queue = log->checkpoint;
  }
  uint64_t i;

  for (i = 0; i < IMPORT_MAX_SIZE; ++i) {
    if (!queue) {
      return count;
    }

    memcpy(&blocks[i], &queue->block, sizeof(block_t));
    blocks[i].used = true;

    queue = queue->next;
    count++;
  }

  if (i != IMPORT_MAX_SIZE) {
    for (; i < IMPORT_MAX_SIZE; ++i) {
      memset(&blocks[i], 0, sizeof(block_t));
    }

    log->checkpoint = NULL;
    return IMPORT_MAX_SIZE - count;
  }

  log->checkpoint = queue->next;
  return count;
}
