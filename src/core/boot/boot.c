#include <args.h>
#include <boot.h>
#include <importer.h>
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

static void boot_quack_core();

void boot_args_init() {}

static void boot_quack_core() { int j; }

void boot_core_init() {
  qlog_infra_init();
  port_boot();
  boot_quack_core();
}

void boot_core(int argc, char **argv) {
  port_init(argc, argv);
  boot_args_init();
  boot_core_init();
}
