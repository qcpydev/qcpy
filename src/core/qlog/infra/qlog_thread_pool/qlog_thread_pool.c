#include <block.h>
#include <exporter.h>
#include <port.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <qlog_thread_pool.h>
#include <stdio.h>

qlog_thread_pool_t qlog_thread_pool;
bool qlog_thread_pool_open = true;

// TODO: switch to using a named pipe, so that we can inter process communicate
// instead of doing these cond waits, for now its "ok"
static void qlog_thread_pool_worker_dequeue_await(block_t *block,
                                                  uint64_t key) {
  while (block_buffer_dequeue(&block_buffer_global_queues->queues[key],
                              block) == QUEUE_STATUS_EMPTY) {
    pthread_cond_wait(&qlog_thread_pool.workers[key].cond,
                      &qlog_thread_pool.workers[key].lock);
  }
}

void *qlog_thread_pool_worker(void *thread_index) {
  uint64_t key = (uint64_t)thread_index;
  qlog_thread_pool.registers[key] = qlog_register_buf_init(NULL);
  qlog_register_buf_t *qlog_buffer_find = qlog_thread_pool.registers[key];

  pthread_t tid = pthread_self();
  block_buffer_queue_id_t queue_id =
      block_buffer_queue_assign_tid(block_buffer_global_queues, tid, key);
  block_t block = {0};

  pthread_mutex_lock(&qlog_thread_pool.workers[key].lock);

  while (qlog_thread_pool_open) {
    buffer_queue_status_e status =
        block_buffer_dequeue(&block_buffer_global_queues->queues[key], &block);

    if (status == QUEUE_STATUS_EMPTY) {
      pthread_mutex_unlock(&qlog_thread_pool.workers[key].lock);
      qlog_thread_pool_worker_dequeue_await(&block, key);
    } else {
      qlog_register_handle_input(qlog_buffer_find,
                                 qlog_thread_pool.registers[key], block);
    }
  }
  pthread_mutex_unlock(&qlog_thread_pool.workers[key].lock);
  return NULL;
}
void qlog_thread_pool_init(uint64_t idx, pthread_attr_t *attr) {
  pthread_cond_init(&qlog_thread_pool.workers[idx].cond, NULL);
  pthread_mutex_init(&qlog_thread_pool.workers[idx].lock, NULL);

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

static void *qlog_thread_pool_admin() {
  for (;;) {
    for (size_t i = 0; i < BLOCK_QUEUES_TOTAL; ++i) {

      block_buffer_queue_t *block_buffer_queue =
          &block_buffer_global_queues->queues[i];
      size_t head =
          atomic_load_explicit(&block_buffer_queue->head, memory_order_relaxed);

      if (head != atomic_load_explicit(&block_buffer_queue->tail,
                                       memory_order_acquire)) {
        if (pthread_mutex_trylock(&qlog_thread_pool.workers[i].lock)) {
          pthread_cond_signal(&qlog_thread_pool.workers[i].cond);

          pthread_mutex_unlock(&qlog_thread_pool.workers[i].lock);
        }
      }
    }
    usleep(BLOCK_FULL_SLEEP);
  }

  return NULL;
}

void qlog_thread_pool_admin_start() {
  pthread_create(&qlog_thread_pool.admin, NULL, qlog_thread_pool_admin, NULL);
}
