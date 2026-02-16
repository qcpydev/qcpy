#include <dock.h>
#include <pthread.h>
#include <stdlib.h>

boot_thread_args_t *boot_thread_args = NULL;

static void qcpy_wait_for_boot() { dock_wait_for_boot(); }

int qcpy_boot_connect(char *args[]) {
  pthread_t boot_thread;
  char *exec_name = dock_init(args);

  boot_thread_args = (boot_thread_args_t *)malloc(sizeof(boot_thread_args_t));
  boot_thread_args->args = args;
  boot_thread_args->exec_name = exec_name;

  pthread_create(&boot_thread, NULL, dock_run_boot, NULL);
  pthread_join(boot_thread, NULL);
  qcpy_wait_for_boot();
  return 0;
}
