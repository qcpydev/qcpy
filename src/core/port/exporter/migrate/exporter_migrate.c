#include <block.h>
#include <exporter_migrate.h>
#include <fcntl.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

exporter_migrate_t *exporter_migrate;
sem_t *exporter_migrate_ready;
sem_t *exporter_migrate_empty;
int exporter_migrate_shared;

void exporter_migrate_init() {
  assert(!exporter_migrate);

  exporter_migrate_shared = shm_open(EXPORTER_MIGRATE_SHARED_MEM,
                                     OFLAG_SHARED_MEM_ARGS, MODE_SHARED_MEM);
  if (exporter_migrate_shared < 0) {
    perror("shm_open");
    _exit(1);
    assert(0);
  }

  if (ftruncate(exporter_migrate_shared, sizeof(import_t)) < 0) {
    perror("ftruncate");
    assert(0);
  }

  exporter_migrate = mmap(NULL, sizeof(*importer), PROT_ARGS, MAP_SHARED,
                          exporter_migrate_shared, 0);

  if (exporter_migrate == MAP_FAILED) {
    perror("map_failed");
    assert(0);
  }

  memset(exporter_migrate, 0, sizeof(import_t));

  sem_unlink(EXPORTER_MIGRATE_READY);
  exporter_migrate_ready = sem_open(EXPORTER_MIGRATE_READY,
                                    OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

  sem_unlink(EXPORTER_MIGRATE_EMPTY);
  exporter_migrate_empty = sem_open(EXPORTER_MIGRATE_EMPTY,
                                    OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

  if (!exporter_migrate_ready) {
    perror("exporter_migrate_ready_failed");
    assert(0);
  }

  if (!exporter_migrate_empty) {
    perror("exporter_migrate_empty_failed");
    assert(0);
  }
}
