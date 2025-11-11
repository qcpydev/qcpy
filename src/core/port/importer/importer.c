#include <importer.h>
#include <qcpy_error.h>
#include <qlog_infra.h>

importer_t import_sorted;

void port_import_init() {
  assert(!import_queue);

  import_queue = mmap(NULL, sizeof(import_sorted_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (!import_queue) {
    assert(0);
  }

  memset(import_queue, 0, sizeof(import_sorted_t));
  memset(&import_queue->queue, 0, sizeof(block_t) * IMPORT_MAX_SIZE);

  sem_init(&import_queue->full, 1, 0);
  sem_init(&import_queue->empty, 1, 1);

  pthread_mutex_init(&import_sorted.sorting, NULL);

  for (uint64_t i = 0; i < IMPORT_MAX_SIZE; ++i) {
    pthread_mutex_init(&import_sorted.registers_lock[i], NULL);
  }

}

imported_block_t* port_imported_init(block_t* ctx) {
  if (!ctx) {
    assert(0);
  }

  imported_block_t* imported_item =
      (imported_block_t *)malloc(sizeof(imported_block_t));

  memset(imported_item, 0, sizeof(imported_block_t));

  imported_item->block = (block_t*)malloc(sizeof(block_t));
  memset(imported_item->block, 0, sizeof(block_t));

  if (!imported_item) {
    assert(0);
  }

  return imported_item;
}

void port_import_enqueue(block_t* block) {
  assert(block && block->type == IMPORTER_QLOG_ENTRY);

  pthread_mutex_lock(&import_sorted.sorting);
  memcpy(&(import_queue->queue[import_queue->size]), block, sizeof(block_t));

  import_queue->size += 1;
  pthread_mutex_unlock(&import_sorted.sorting);
}

uint64_t port_import_sort(block_t* block) {
  if (!block) {
    assert(0);
  }

  imported_block_t *imported_block = port_imported_init(block);
  uint64_t reg = imported_block->block->content.qlog_register % IMPORT_MAX_SIZE;

  if (import_sorted.registers[reg]) {
    import_sorted.registers_last[reg]->next_block = imported_block;
    import_sorted.registers_last[reg] = imported_block;
  }
  else {
    import_sorted.registers[reg] = imported_block;
    import_sorted.registers_last[reg] = imported_block;
    ++(import_sorted.count);
  }

  return reg;
}

uint64_t port_import_handler() {
  uint64_t sorted_cnt = 0;

  pthread_mutex_lock(&import_sorted.sorting);
  port_import_clear();
  pthread_mutex_unlock(&import_sorted.sorting);
  return 1;

  for (uint64_t i = 0; i < IMPORT_MAX_SIZE; ++i) {
    uint64_t reg = port_import_sort(&(import_queue->queue[i]));

    pthread_mutex_lock(&import_sorted.registers_lock[reg]);

    qlog_thread_pool.workers[i].state = QLOG_PROCESS_START;
    qlog_thread_pool_signal_worker(i);

    pthread_mutex_unlock(&import_sorted.registers_lock[i]);
  }

  sorted_cnt = import_sorted.count;
  return sorted_cnt;
}

void port_import_clear() {
  if (!import_queue) {
    assert(0);
  }

  memset(&import_queue->queue, 0, sizeof(block_t) * IMPORT_MAX_SIZE);
  import_queue->size = 0;
}
