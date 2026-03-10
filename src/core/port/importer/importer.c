#include <block.h>
#include <importer.h>
#include <port.h>
#include <qcpy_error.h>
#include <qlog_infra.h>

import_sort_t importer_sort;

void importer_init() {
  shared_import_space = shm_open(QCPY_IMPORT, O_RDWR, MODE_SHARED_MEM);

  if (shared_import_space == -1) {
    perror("shm_open child");
    exit(1);
  }

  importer = mmap(NULL, sizeof(*importer), PROT_ARGS, MAP_SHARED,
                  shared_import_space, 0);
  if (importer == MAP_FAILED) {
    assert(0);
  }

  dock_import_sem = sem_open(DOCK_IMPORT_SEM, 0);
  if (dock_import_sem == SEM_FAILED) {
    perror("sem_open");
    assert(0);
  }

  port_import_sem = sem_open(PORT_IMPORT_SEM, 0);

  if (port_import_sem == SEM_FAILED) {
    perror("sem_open");
    assert(0);
  }

  importer->ready = true;
}

void importer_sort_init() {
  pthread_mutex_init(&importer_sort.lock, NULL);

  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    pthread_mutex_init(&importer_sort.queue_lock[i], NULL);
  }

  memset(importer_sort.queue, 0, sizeof(import_block_t *) * IMPORTER_FUNNEL);
  memset(importer_sort.queue_last, 0,
         sizeof(import_block_t *) * IMPORTER_FUNNEL);
}

void importer_sort_append(block_t block) {
  import_block_t *import_block = import_block_init();
  assert(import_block);
  uint64_t index = block.reg % IMPORTER_FUNNEL;

  if (!importer_sort.queue[index]) {
    importer_sort.queue[index] = import_block;
    importer_sort.queue_last[index] = import_block;
  } else {
    importer_sort.queue_last[index]->next = import_block;
    importer_sort.queue_last[index] = importer_sort.queue_last[index]->next;
  }

  ++importer_sort.queue_count[index];
  ++importer_sort.count;
}

void importer_sort_ported(import_t *importer) {
  assert(importer);

  sem_wait(dock_import_sem);

  for (uint64_t i = 0; i < IMPORT_MAX_SIZE; ++i) {
    if (importer->queue[i].used) {
      importer_sort_append(importer->queue[i]);
      importer->queue[i].used = false;
    }
  }

  importer->idx = 0;

  qlog_infra_process(&importer_sort);

  sem_post(port_import_sem);
}

void importer_delete_queue(uint64_t idx) {
  assert(idx < IMPORTER_FUNNEL);

  import_block_t *import_block_queue = importer_sort.queue[idx];
  importer_sort.queue[idx] = NULL;
  importer_sort.queue_last[idx] = NULL;

  while (import_block_queue) {
    import_block_t *delete_block = import_block_queue;
    import_block_queue = import_block_queue->next;
    import_block_delete(delete_block);
  }

  importer_sort.queue_count[idx] = 0;
}

void importer_sort_clear() {
  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    importer_delete_queue(i);
  }

  importer_sort.count = 0;
}

import_block_t *import_block_init() {
  import_block_t *import_block;
  import_block = (import_block_t *)malloc(sizeof(import_block_t));
  memset(import_block, 0, sizeof(import_block_t));
  assert(import_block);

  return import_block;
}

void import_block_delete(import_block_t *import_block) {
  assert(import_block);
  memset(import_block, 0, sizeof(import_block_t));
  free(import_block);
  return;
}
