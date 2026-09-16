#include <qlog_infra.h>
#include <sched.h>

void qlog_infra_init() {
  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    qlog_thread_pool_init(i, &qlog_scheduler.attributes[i]);
  }
}

qlog_t *qlog_infra_find_qlog(int reg) {
  assert(reg != -1);

  qlog_t *qlog = qlog_thread_pool_get_qlog(reg);

  assert(qlog);
  return qlog;
}

void qlog_infra_await_finish() {
  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    qlog_thread_join(i);
  }
}
