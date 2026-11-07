#include <port.h>
#include <qlog.h>
#include <qlog_infra.h>
#include <qlog_register.h>
#include <stdio.h>

/*
 * Boot process for qcpy_core
 * This will spawn a subprocess, which will actually commit to the work
 * as a consumer of what the user inputs from the front end of the API
 * Here, we expect to create a shared memory space to transfer from
 * the main API process to the backend process.
 *
 * This space will also consider arguments that the user has included,
 * such as utilization of GPU over CPU, disable the compiler, etc.
 */

void boot_core_init() {
  qlog_infra_init();
  qlog_infra_await_finish();
}

void boot_core(int argc, char **argv) {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  port_init(argc, argv);
  boot_core_init();
}
