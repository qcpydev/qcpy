#include <assert.h>
#include <exporter.h>
#include <port.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

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

  exporter->qcpy_core_ready = true;
}

void exporter_quack(export_t *export) {
  assert(export);
  if (!export) {
  }
}
