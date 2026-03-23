#include <qlog_infra.h>
#include <sched.h>

void qlog_infra_init() {
  qlog_scheduler_init();

  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    qlog_thread_pool_init(i, &qlog_scheduler.attributes[i]);
  }
}

void qlog_infra_set_priority(import_sort_t *importer_sort) {
  assert(importer_sort);

  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    if (importer_sort->queue_count[i] == 0) {
      continue;
    }

    uint32_t weight_priority =
        (100 * importer_sort->queue_count[i]) / importer_sort->count;

    if (weight_priority > QLOG_SCHED_MAX_PRIORITY) {
      weight_priority = QLOG_SCHED_MAX_PRIORITY;
    } else if (weight_priority < QLOG_SCHED_MIN_PRIORITY) {
      weight_priority = QLOG_SCHED_MIN_PRIORITY;
    }

    qlog_scheduler_set(&qlog_thread_pool.workers[i].thread, weight_priority, i);
  }
}

void qlog_infra_await_completion() {
  bool still_running = true;
  while (still_running) {
    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
      pthread_mutex_lock(&qlog_thread_pool.workers[i].lock);

      still_running =
          !(qlog_thread_pool.workers[i].state == QLOG_PROCESS_EMPTY ||
            qlog_thread_pool.workers[i].state == QLOG_PROCESS_DONE);

      if (still_running) {
        break;
      }

      pthread_mutex_unlock(&qlog_thread_pool.workers[i].lock);
    }
  }
}

void qlog_infra_await_completion_reg(int reg) {
  assert(reg != -1);
  uint64_t key = reg % IMPORTER_FUNNEL;

  bool completed = false;

  while (!completed) {
    completed = qlog_thread_pool.workers[key].state &=
        QLOG_PROCESS_READY | QLOG_PROCESS_DONE;
  }

  qlog_thread_pool.workers[key].state = QLOG_PROCESS_DONE;
}

void qlog_infra_process(import_sort_t *importer_sort) {
  qlog_infra_set_priority(importer_sort);

  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    if (importer_sort->queue[i]) {

      if (importer->flushing && importer->flush_reg % IMPORTER_FUNNEL != i) {
        continue;
      }

      qlog_thread_pool_signal_worker(i);
    }
  }
}

qlog_t *qlog_infra_find_qlog(int reg) {
  assert(reg != -1);

  qlog_t *qlog = qlog_thread_pool_get_qlog(reg);

  assert(qlog);
  return qlog;
}
