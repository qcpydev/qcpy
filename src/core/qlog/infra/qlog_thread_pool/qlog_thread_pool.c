#include <qcpy_error.h>
#include <qlog_thread_pool.h>

qlog_thread_pool_t qlog_thread_pool;
bool qlog_thread_pool_open = false;

void* qlog_thread_pool_worker(void* thread_index) {
  uint64_t key = (uint64_t)thread_index;

  pthread_mutex_lock(&import_sorted.registers_lock[key]);

  while (qlog_thread_pool.workers[key].state != QLOG_PROCESS_START) {
    pthread_cond_wait(&qlog_thread_pool.workers[key].cond, &import_sorted.registers_lock[key]);
    qlog_thread_pool.workers[key].state = QLOG_PROCESS_DONE;

  }

  pthread_mutex_unlock(&import_sorted.registers_lock[key]);

  return NULL;
}

void qlog_thread_pool_init() {
  pthread_mutex_init(&(qlog_thread_pool.lock), NULL);

  for (uint64_t i = 0; i < IMPORT_MAX_SIZE; ++i) {
    pthread_cond_init(&qlog_thread_pool.workers[i].cond, NULL);
    pthread_create(&qlog_thread_pool.workers[i].thread, NULL, qlog_thread_pool_worker, (void *)(intptr_t)i);
  }
}

void qlog_thread_pool_signal_worker(uint64_t key) {
  pthread_cond_signal(&qlog_thread_pool.workers[key].cond);
}

