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

void dock_wait_for_boot() {
  while (!importer || !exporter || !importer->ready ||
         !exporter->qcpy_core_ready) {
  }
}

static void dock_create_importer() {
  importer =
      (import_t *)base_tools_create_shared_mem(sizeof(import_t), QCPY_IMPORT);
  assert(importer);

  port_import_sem = base_tools_create_shared_sem(PORT_IMPORT_SEM_ONE, 0);
  assert(port_import_sem);

  dock_import_sem = base_tools_create_shared_sem(DOCK_IMPORT_SEM_ONE, 0);
  assert(dock_import_sem);
}

static void dock_create_exporter() {
  assert(!exporter);
  exporter =
      (export_t *)base_tools_create_shared_mem(sizeof(export_t), QCPY_EXPORT);

  port_export_sem = base_tools_create_shared_sem(PORT_EXPORT_SEM, 0);
  assert(port_export_sem);

  dock_export_sem = base_tools_create_shared_sem(DOCK_EXPORT_SEM, 0);
  assert(dock_export_sem);
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

static void dock_flush_entries() {
  sem_post(dock_import_sem);
  sem_wait(port_import_sem);
  assert(importer->idx == 0);
}

int dock_add(block_t *block) {
  // this is the problem and it sucks here, we can do better.
  // dock_log_append(block->reg, block);

  block_add(block, importer);
  if (importer->idx == IMPORT_MAX_SIZE) {
    dock_flush_entries();
  }

  return 1;
}

void dock_get_qc_state(int flush_reg) {

  if (importer->idx > 0) {
    importer->flushing = true;
    importer->flush_reg = flush_reg;

    // come back to this later, we are causing some shitty perf issues with
    // allowing the dock_logger to run like that
    // most likely we will need to break some rules potentially, and have a
    // grabby process to grab info from the qcpy_core that isnt important,
    // qcpy_logger?
    //   dock_flush_entries();
  }

  /*
   * TODO: await for exporter to let us know it is ready for us to consume
   * data, use a do while loop to await for entries to be calculated
   */
}
