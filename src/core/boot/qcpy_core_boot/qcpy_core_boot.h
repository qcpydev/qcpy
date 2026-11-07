#pragma once
#ifndef BOOT
#define BOOT

typedef enum {
  BOOT_SUCCESS,
  BOOT_ERROR_ARGS,
  BOOT_ERROR_START,
  BOOT_ERROR_UNKNOWN,
} boot_complete_e;

int boot_connect();
void boot_core_start();
void boot_core_init();
void boot_core(int argc, char **argv);
void boot_args_init();
#endif
