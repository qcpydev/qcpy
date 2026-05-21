#include <dock.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

boot_thread_args_t *boot_thread_args = NULL;

static void qcpy_wait_for_boot() { dock_wait_for_boot(); }

int qcpy_boot_connect(char *args[]) {
  char *exec_name = dock_init(args);
  char *quack_name = dock_quack_init(args);
  char *quack_gpu_name = dock_quack_gpu_init(args);
  bool is_gpu_enabled = dock_set_gpu_enabled(args);

  boot_thread_args = (boot_thread_args_t *)malloc(sizeof(boot_thread_args_t));
  boot_thread_args->args = args;
  boot_thread_args->exec_name = exec_name;
  boot_thread_args->quack_exec_name = quack_name;
  boot_thread_args->quack_gpu_exec_name = quack_gpu_name;
  boot_thread_args->is_gpu_enabled = is_gpu_enabled;

  // this should be done differently IMO, dock should be for adding entries and
  // logger
  dock_run_boot();
  qcpy_wait_for_boot();
  return 0;
}
