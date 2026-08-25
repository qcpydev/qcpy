#include <block.h>
#include <exporter.h>
#include <qcpy_error.h>
#include <qlog_thread_pool.h>
#include <stdio.h>

qlog_thread_pool_t qlog_thread_pool;
bool qlog_thread_pool_open = true;

bool ready[IMPORTER_FUNNEL];

static inline void qlog_thread_pool_exporter_signal(uint64_t key) {
  if (!importer->flushing || importer->flush_reg % IMPORTER_FUNNEL != key) {
    return;
  }
  assert(!"what...\n");

  exporter_signal_item_append(importer->flush_reg);

  pthread_cond_signal(&exporter_signal.cond);
}

void *qlog_thread_pool_worker(void *thread_index) {
  uint64_t key = (uint64_t)thread_index;
  qlog_thread_pool.registers[key] = qlog_register_buf_init(NULL);

  qlog_register_buf_t *qlog_buffer_find = qlog_thread_pool.registers[key];

  pthread_mutex_lock(&qlog_thread_pool.workers[key].lock);
  uint64_t total = 0;

  while (qlog_thread_pool_open) {
    while (!importer_sort.queue_count[key]) {
      pthread_cond_wait(&qlog_thread_pool.workers[key].cond,
                        &qlog_thread_pool.workers[key].lock);
    }

    qlog_thread_pool.workers[key].state = QLOG_PROCESS_START;

    pthread_mutex_lock(&importer_sort.queue_lock[key]);
    assert(importer_sort.queue_count[key] && importer_sort.queue[key]);
    uint64_t count = importer_sort.queue_count[key];
    total += count;
    pthread_mutex_unlock(&importer_sort.queue_lock[key]);

    for (uint64_t i = 0; i < count; ++i) {
      qlog_thread_pool.workers[key].state = QLOG_PROCESS_APPENDING;
      import_block_t *import_block = import_block_dequeue(key);

      qlog_register_buf_append_handler(qlog_buffer_find,
                                       qlog_thread_pool.registers[key],
                                       import_block->block);

      import_block_delete(import_block);
    }

    // qlog_thread_pool_exporter_signal(key);

    qlog_thread_pool.workers[key].state = QLOG_PROCESS_READY;
  }

  pthread_mutex_unlock(&qlog_thread_pool.workers[key].lock);

  return NULL;
}

void qlog_thread_pool_init(uint64_t idx, pthread_attr_t *attr) {
  pthread_mutex_init(&qlog_thread_pool.workers[idx].lock, NULL);
  pthread_create(&qlog_thread_pool.workers[idx].thread, attr,
                 qlog_thread_pool_worker, (void *)(intptr_t)idx);
}

void qlog_thread_pool_signal_worker(uint64_t key) {
  pthread_cond_signal(&qlog_thread_pool.workers[key].cond);
}

qlog_t *qlog_thread_pool_get_qlog(uint32_t reg) {
  uint64_t key = reg % IMPORTER_FUNNEL;
  assert(qlog_thread_pool.registers[key]);

  qlog_register_buf_t *qlog_reg_buffer =
      qlog_register_buf_find(qlog_thread_pool.registers[key], reg);

  assert(qlog_reg_buffer);

  return qlog_reg_buffer->reg.qlog;
}
