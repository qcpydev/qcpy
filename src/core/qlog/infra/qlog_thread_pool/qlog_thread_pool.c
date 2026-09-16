#include <block.h>
#include <exporter.h>
#include <port.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <qlog_thread_pool.h>
#include <stdio.h>

qlog_thread_pool_t qlog_thread_pool;
bool qlog_thread_pool_open = true;

void *qlog_thread_pool_worker(void *thread_index) {
  uint64_t key = (uint64_t)thread_index;
  qlog_thread_pool.registers[key] = qlog_register_buf_init(NULL);
  qlog_register_buf_t *qlog_buffer_find = qlog_thread_pool.registers[key];

  pthread_t tid = pthread_self();
  block_buffer_queue_id_t queue_id =
      block_buffer_queue_assign_tid(block_buffer_global_queues, tid, key);

  while (qlog_thread_pool_open) {
    block_t block = block_dequeue(block_buffer_global_queues, queue_id);
    qlog_register_buf_append_handler(qlog_buffer_find,
                                     qlog_thread_pool.registers[key], block);
  }
  return NULL;
}

void qlog_thread_pool_init(uint64_t idx, pthread_attr_t *attr) {
  pthread_create(&qlog_thread_pool.workers[idx].thread, NULL,
                 qlog_thread_pool_worker, (void *)(intptr_t)idx);
}

void qlog_thread_join(uint64_t idx) {
  pthread_join(qlog_thread_pool.workers[idx].thread, NULL);
}

qlog_t *qlog_thread_pool_get_qlog(uint32_t reg) {
  uint64_t key = reg % IMPORTER_FUNNEL;
  assert(qlog_thread_pool.registers[key]);

  qlog_register_buf_t *qlog_reg_buffer =
      qlog_register_buf_find(qlog_thread_pool.registers[key], reg);

  assert(qlog_reg_buffer);

  return qlog_reg_buffer->reg.qlog;
}
