#include <args.h>
#include <boot.h>
#include <port.h>
#include <qlog.h>
#include <qlog_infra.h>
#include <qlog_register.h>

/*
 * Boot process for qcpy
 * This will spawn a subprocess, which will actually commit to the work
 * as a consumer of what the user inputs from the front end of the API
 * Here, we expect to create a shared memory space to transfer from
 * the main API process to the backend process.
 *
 * This space will also consider arguments that the user has included,
 * such as utilization of GPU over CPU, disable the compiler, etc.
 */

// TODO: SET BOOT ARGS TO TAKE IN CHAR ARRAY

void boot_args_init() {}

void boot_core_init() {
  pthread_create(&ports.import_thread, NULL, port_import, NULL);
  qlog_infra_init();
}

void boot_core() {
  boot_args_init();
}
