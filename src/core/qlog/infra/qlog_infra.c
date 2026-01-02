#include <qlog_infra.h>
#include <sched.h>

void qlog_infra_init() {
  qlog_thread_pool_init();
  qlog_scheduler_init();
}
