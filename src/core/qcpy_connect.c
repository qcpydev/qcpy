#include <dock.h>

int qcpy_boot_connect(char* args[]) {
  char* exec_name = dock_init(args);
  dock_run_boot(exec_name, args);
  return 0;
}

