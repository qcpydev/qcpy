#include <assert.h>
#include <qcpy_error.h>
#include <qlog_scheduler.h>
#include <stdio.h>
#include <unistd.h>

qlog_scheduler_t qlog_scheduler;

void qlog_scheduler_init() {
  qlog_scheduler.policy = QLOG_SCHED_POLICY;

  for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i) {
    int set_attr = pthread_attr_init(&qlog_scheduler.attributes[i]);

    if (set_attr) {
      perror("pthread_attr_init failed");
    }

    set_attr = pthread_attr_setschedpolicy(&qlog_scheduler.attributes[i],
                                           QLOG_SCHED_POLICY);

    if (set_attr) {
      perror("pthread_attr_setschedpolicy");
    }

    set_attr = pthread_attr_setschedparam(&qlog_scheduler.attributes[i],
                                          &qlog_scheduler.params[i]);
  }
}

void qlog_scheduler_set(pthread_t *qlog_worker, int weight_priority,
                        uint64_t idx) {
  assert(qlog_worker && weight_priority <= QLOG_SCHED_MAX_PRIORITY &&
         weight_priority >= QLOG_SCHED_MIN_PRIORITY);
  int current_policy = 0;
  pthread_getschedparam(*qlog_worker, &current_policy,
                        &qlog_scheduler.params[idx]);

  (void)pthread_setschedprio(*qlog_worker, (int)weight_priority);

  int min_prio = sched_get_priority_min(SCHED_RR);
  int max_prio = sched_get_priority_max(SCHED_RR);

  if (weight_priority >= min_prio || weight_priority <= max_prio) {
    (void)pthread_setschedprio(*qlog_worker, weight_priority);
  }
}
