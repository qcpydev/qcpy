#include <block.h>
#include <exporter_migrate.h>
#include <fcntl.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

exporter_migrate_t *exporter_migrate;
sem_t *exporter_migrate_ready;
sem_t *exporter_migrate_empty;
int exporter_migrate_shared;
volatile _Atomic int counter = 0;

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

static bool exporter_migrate_single_work(qlog_node_t **qlog_nodes,
                                         uint32_t size) {
  assert(qlog_nodes);
  bool single_work = false;
  for (uint32_t i = 0; i < size; ++i) {
    if (qlog_nodes[i] && (!qlog_nodes[i]->up && !qlog_nodes[i]->down)) {
      single_work = true;
      qlog_nodes[i] = qlog_nodes[i]->next;
    }
  }

  return single_work;
}

static bool exporter_migrate_multi_work(qlog_node_t **qlog_nodes,
                                        uint32_t size) {
  assert(qlog_nodes);
  bool multi_work = false;
  for (uint32_t i = 0; i < size; ++i) {
    if (qlog_nodes[i] && (qlog_nodes[i]->up || qlog_nodes[i]->down)) {
      multi_work = true;

      if (qlog_nodes[i]->up) {
        qlog_nodes[i]->up = qlog_nodes[i]->up->next;
      }

      if (qlog_nodes[i]->down) {
        qlog_nodes[i]->down = qlog_nodes[i]->down->next;
      }

      qlog_nodes[i] = qlog_nodes[i]->next;
    }
  }
  return multi_work;
}

void exporter_migrate_fill_queue(qlog_node_t **qlog_nodes, uint32_t size) {

  assert(qlog_nodes);

  bool work_to_do = false;
  do {
    bool single_work = false;
    do {
      single_work = exporter_migrate_single_work(qlog_nodes, size);
    } while (single_work);

    bool multi_work = false;

    do {
      multi_work = exporter_migrate_multi_work(qlog_nodes, size);
    } while (multi_work);

    work_to_do = single_work || multi_work;
  } while (work_to_do);
}
