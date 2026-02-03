#include <qlog_infra.h>

#pragma once
#ifndef BOOT
#define BOOT

/*
 * Boot is called by the interface when the Python process is ran. This segment
 * will "boot" needed processes, thread pools, etc to operate qcpy.
 * Boot segment of qcpy will also set flags that are passed into the boot via a
 * int array.
 */

typedef enum
{
    BOOT_SUCCESS,
    BOOT_ERROR_ARGS,
    BOOT_ERROR_START,
    BOOT_ERROR_UNKNOWN,
} boot_complete_e;

int boot_connect();
void boot_core_start();
void boot_core_init();
void boot_core(int argc, char** argv);
void boot_args_init();
#endif
