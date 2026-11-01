#include <dock.h>
#include <pthread.h>

int qcpy_boot_connect(char *args[]) {
  dock_setup_early_init(args);
  return 0;
}
