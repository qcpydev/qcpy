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

bootarg_wrapper_t qcpy_bootarg_wrapper;

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

static void dock_port_quack_init() {
  assert(!exporter);
  shared_export_space = base_tools_create_shared_mem(
      sizeof(export_t), QCPY_EXPORT, (void **)&exporter);
  assert(exporter && shared_export_space != -1);

  temp_migrate_memfd = base_tools_create_shared_mem(
      sizeof(exporter_migrate_t), EXPORTER_MIGRATE_SHARED_MEM,
      (void **)&exporter_migrate_watcher);
}

static void dock_port_qcpy_core_init() {
  assert(!block_buffer_global_queues);

  global_queues_shared_fd = base_tools_create_shared_mem(
      sizeof(block_buffer_global_t), BLOCK_BUFFER_QUEUE_GLOBAL_NAME,
      (void **)&block_buffer_global_queues);

  assert(block_buffer_global_queues);
  assert(global_queues_shared_fd != -1);
}

inline static void dock_port_setup_get_qc_key() { srand(time(NULL)); }

void dock_setup_global_shared_mem() {
  dock_port_qcpy_core_init();
  dock_port_quack_init();
}

static void dock_run_qcpy_core_boot() {
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

    char *args_test[] = {qcpy_bootarg_wrapper.qcpy_core_exec_name,
                         buffer_queues_memfd_id, exporter_memfd_id,
                         exporter_migrate_memfd, NULL};
    execvp(qcpy_bootarg_wrapper.qcpy_core_exec_name, args_test);
    perror("execvp failed");
    _exit(1);
  }

  dock_wait_for_qcpy_core_boot();
}

static void dock_wait_for_quack_boot() {}

static void dock_run_quack_boot() {
  pid_t quack_pid = fork();
  if (quack_pid == 0) {
    prctl(PR_SET_PDEATHSIG, SIGTERM);

    char exporter_migrate_memfd[20];
    snprintf(exporter_migrate_memfd, sizeof(exporter_migrate_memfd), "%d",
             temp_migrate_memfd);

    char *args_test[] = {qcpy_bootarg_wrapper.quack_exec_name,
                         exporter_migrate_memfd, NULL};

    char *run_quack_version = qcpy_bootarg_wrapper.quack_exec_name;

    execvp(run_quack_version, args_test);
    perror("execvp failed");
    _exit(1);
  }

  dock_wait_for_quack_boot();
}

void dock_run_boot() {
  dock_setup_global_shared_mem();
  dock_port_setup_get_qc_key();

  dock_run_qcpy_core_boot();
  dock_run_quack_boot();
}

void dock_create_random_id(block_t *block) {
  block->qubit_bitmask = rand();
  block->qubit_bitmask |= (size_t)rand() << (sizeof(int));
}

void dock_get_qc_print(uint16_t flush_reg) {
  block_t *temp_block = (block_t *)calloc(1, sizeof(block_t));
  assert(temp_block);
  temp_block->reg = flush_reg;
  temp_block->type = BLOCK_PRINT_ENTRY;

  dock_create_random_id(temp_block);
  block_enqueue(block_buffer_global_queues, temp_block);

  usleep(4000);
}

void dock_get_qc_state(uint16_t flush_reg) {
  block_t *temp_block = (block_t *)calloc(1, sizeof(block_t));
  assert(temp_block);
  temp_block->reg = flush_reg;
  temp_block->type = BLOCK_FLUSH_ENTRY;

  dock_create_random_id(temp_block);
  block_enqueue(block_buffer_global_queues, temp_block);

  // TODO: await for unique rand id to be stored back into the exporter data,
  // letting us know that this round of statevector info  is uniquely ours in
  // this routine.

  usleep(4000);
}

void dock_add(block_t *block) {
  block_enqueue(block_buffer_global_queues, block);
}

static void dock_run_setup_qcpy_core_bootargs(char *args[]) {
  assert(args && args[0]);
  qcpy_bootarg_wrapper.qcpy_core_exec_name = args[QCPY_BOOTARG_CORE_EXEC_NAME];
}

static void dock_run_setup_quack_bootargs(char *args[]) {
  assert(args && args[0]);
  qcpy_bootarg_wrapper.quack_exec_name = args[QCPY_BOOTARG_QUACK_EXEC_NAME];
  qcpy_bootarg_wrapper.quack_gpu_exec_name =
      args[QCPY_BOOTARG_QUACK_GPU_EXEC_NAME];
  qcpy_bootarg_wrapper.is_gpu_enabled = args[QCPY_BOOTARG_IS_GPU_ENABLED];
}

static void dock_run_setup_bootargs(char *args[]) {
  assert(args && args[0]);
  dock_run_setup_qcpy_core_bootargs(args);
  dock_run_setup_quack_bootargs(args);
}

void dock_setup_early_init(char *args[]) {
  assert(args && args[0]);

  dock_run_setup_bootargs(args);
  dock_run_boot();
}
