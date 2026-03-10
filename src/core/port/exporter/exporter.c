#include <assert.h>
#include <exporter.h>
#include <port.h>
#include <pthread.h>
#include <qlog_infra.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

export_signal_t *export_signal;
sem_t *export_sig_stop;
sem_t *export_sig_ready;
int shared_export_signal_space;

static void exporter_signal_open() {
  shared_export_signal_space =
      shm_open(EXPORT_SIGNAL_SHARED_MEM, O_RDWR, MODE_SHARED_MEM);

  if (shared_export_space == -1) {
    perror("shm_open child");
    exit(1);
  }

  export_signal = mmap(NULL, sizeof(*export_signal), PROT_ARGS, MAP_SHARED,
                       shared_export_signal_space, 0);
  if (export_signal == MAP_FAILED) {
    assert(0);
  }

  memset(export_signal, 0, sizeof(export_signal_t));

  export_sig_stop = sem_open(EXPORT_SIGNAL_STOP, 0);
  if (export_sig_stop == SEM_FAILED) {
    perror("sem_open");
    assert(0);
  }

  export_sig_ready = sem_open(EXPORT_SIGNAL_READY, 0);
  if (export_sig_ready == SEM_FAILED) {
    perror("sem_open");
    assert(0);
  }

  export_signal->reg = -1;
  export_signal->qcpy_core_ready = true;
}

void exporter_await_quack_start() {}

// TODO: MOVE THIS TO QUACK_EXPORT. We will never actually need to use this
// shared mem, so whats the point of opening it! We will use our own shared mem
// for exporter to send info to quack. We are just the mediator for sending over
// stuff to quack

void exporter_init() {
  shared_export_space = shm_open(QCPY_EXPORT, O_RDWR, MODE_SHARED_MEM);

  if (shared_export_space == -1) {
    perror("shm_open child");
    exit(1);
  }

  exporter = mmap(NULL, sizeof(*exporter), PROT_ARGS, MAP_SHARED,
                  shared_export_space, 0);

  if (exporter == MAP_FAILED) {
    assert(0);
  }

  dock_export_sem = sem_open(DOCK_EXPORT_SEM, 0);
  if (dock_export_sem == SEM_FAILED) {
    perror("sem_open");
    assert(0);
  }

  port_export_sem = sem_open(PORT_EXPORT_SEM, 0);

  if (port_export_sem == SEM_FAILED) {
    perror("sem_open");
    assert(0);
  }

  exporter_signal_open();
  exporter_await_quack_start();

  exporter->qcpy_core_ready = true;
}

void exporter_process() {
  sem_wait(export_sig_ready);

  qlog_infra_await_completion();
  export_signal->reg = -1;

  sem_post(export_sig_stop);
}
