#include <block.h>
#include <exporter.h>
#include <qcpy_error.h>
#include <qlog_thread_pool.h>

qlog_thread_pool_t qlog_thread_pool;
bool qlog_thread_pool_open = true;

void *qlog_thread_pool_worker(void *thread_index) {
  uint64_t key = (uint64_t)thread_index;
  qlog_thread_pool.registers[key] = qlog_register_buf_init(NULL);

  qlog_register_buf_t *qlog_buffer_find = qlog_thread_pool.registers[key];

  pthread_mutex_lock(&qlog_thread_pool.workers[key].lock);

  while (qlog_thread_pool_open) {
    pthread_cond_wait(&qlog_thread_pool.workers[key].cond,
                      &qlog_thread_pool.workers[key].lock);

    if (importer->flushing && importer->flush_reg % IMPORTER_FUNNEL == key) {
      pthread_mutex_lock(&exporter_signal.lock);
    }

    qlog_thread_pool.workers[key].state = QLOG_PROCESS_START;

    import_block_t *block_queue = importer_sort.queue[key];
    assert(block_queue);
    while (block_queue) {
      qlog_thread_pool.workers[key].state = QLOG_PROCESS_APPENDING;
      qlog_register_buf_append_handler(qlog_buffer_find,
                                       qlog_thread_pool.registers[key],
                                       block_queue->block);
      block_queue = block_queue->next;
    }

    importer_delete_queue(key);

    if (importer->flushing && importer->flush_reg % IMPORTER_FUNNEL == key) {
      qlog_thread_pool.workers[key].state = QLOG_PROCESS_READY;
      exporter_signal_item_append(importer->flush_reg);

      importer->flushing = false;
      importer->flush_reg = -1;
      pthread_mutex_unlock(&exporter_signal.lock);
      printf("signaling exporter_signal\n");
      pthread_cond_signal(&exporter_signal.cond);
    }

    qlog_thread_pool.workers[key].state = QLOG_PROCESS_EMPTY;
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
