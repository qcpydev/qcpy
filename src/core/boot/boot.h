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
void boot();
void boot_core_init();
void boot_args_init();
#endif
