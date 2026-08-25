#include <base_tools.h>
#include <block.h>
#include <importer.h>
#include <port.h>
#include <qcpy_error.h>
#include <qlog_infra.h>
#include <stdio.h>
#include <string.h>

import_t *importer;

sem_t *dock_import_sem;
sem_t *port_import_sem;

import_sort_t importer_sort;

void importer_open() {
  importer =
      (import_t *)base_tools_open_shared_mem(sizeof(import_t), QCPY_IMPORT);
  assert(importer);

  dock_import_sem = base_tools_open_shared_sem(DOCK_IMPORT_SEM);
  assert(dock_import_sem);

  port_import_sem = base_tools_open_shared_sem(PORT_IMPORT_SEM);
  assert(port_import_sem);

  importer->ready = true;
}

void importer_init() {
  importer =
      (import_t *)base_tools_create_shared_mem(sizeof(import_t), QCPY_IMPORT);
  assert(importer);

  port_import_sem = base_tools_create_shared_sem(PORT_IMPORT_SEM, 0);
  assert(port_import_sem);

  dock_import_sem = base_tools_create_shared_sem(DOCK_IMPORT_SEM, 0);
  assert(dock_import_sem);
}

void importer_sort_init() {

  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    pthread_mutex_init(&importer_sort.queue_lock[i], NULL);
  }

  memset(importer_sort.queue, 0, sizeof(import_block_t *) * IMPORTER_FUNNEL);
  memset(importer_sort.queue_last, 0,
         sizeof(import_block_t *) * IMPORTER_FUNNEL);
}

void importer_sort_append(block_t block) {
  import_block_t *import_block = import_block_init(block);
  assert(import_block);

  uint64_t index = block.reg % IMPORTER_FUNNEL;

  pthread_mutex_lock(&importer_sort.queue_lock[index]);
  if (!importer_sort.queue[index]) {
    importer_sort.queue[index] = import_block;
    importer_sort.queue_last[index] = import_block;
  } else {
    importer_sort.queue_last[index]->next = import_block;
    importer_sort.queue_last[index] = importer_sort.queue_last[index]->next;
  }

  ++importer_sort.queue_count[index];
  ++importer_sort.count;
  pthread_mutex_unlock(&importer_sort.queue_lock[index]);
}

void importer_sort_ported(import_t *importer) {
  assert(importer);

  sem_wait(dock_import_sem);

  for (uint64_t i = 0; i < importer->idx; ++i) {
    if (importer->queue[i].used) {
      assert(importer->queue[i].qubits != 0);

      importer_sort_append(importer->queue[i]);

      importer->queue[i].used = false;
    }
  }

  qlog_infra_process(&importer_sort);

  importer->idx = 0;

  sem_post(port_import_sem);
}

void importer_delete_queue(uint64_t idx, uint64_t count) {
  assert(idx < IMPORTER_FUNNEL);

  import_block_t *import_block_queue = importer_sort.queue[idx];
  importer_sort.queue[idx] = NULL;
  importer_sort.queue_last[idx] = NULL;

  for (uint64_t i = 0; i < importer_sort.queue_count[idx]; ++i) {
    import_block_t *delete_block = import_block_queue;
    import_block_queue = import_block_queue->next;
    import_block_delete(delete_block);
  }

  importer_sort.queue_count[idx] = 0;
}

import_block_t *import_block_init(block_t block) {
  import_block_t *import_block;
  import_block = (import_block_t *)malloc(sizeof(import_block_t));
  memset(import_block, 0, sizeof(import_block_t));
  assert(import_block);
  import_block->block = block;

  return import_block;
}

import_block_t *import_block_dequeue(uint64_t index) {
  import_block_t *import_block = NULL;

  pthread_mutex_lock(&importer_sort.queue_lock[index]);

  assert(importer_sort.queue_count[index]);
  import_block = importer_sort.queue[index];
  importer_sort.queue[index] = importer_sort.queue[index]->next;
  if (importer_sort.queue_last[index] == import_block) {
    importer_sort.queue_last[index] = importer_sort.queue[index];
  }
  importer_sort.queue_count[index]--;

  pthread_mutex_unlock(&importer_sort.queue_lock[index]);

  assert(import_block);
  return import_block;
}

void import_block_delete(import_block_t *import_block) {
  assert(import_block);
  memset(import_block, 0, sizeof(import_block_t));
  free(import_block);
  return;
}
