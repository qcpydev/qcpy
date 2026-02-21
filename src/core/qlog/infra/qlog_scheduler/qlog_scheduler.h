#include <block.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>

#pragma once
#ifndef QLOG_SCHEDULER_H
#define QLOG_SCHEDULER_H

/*
 * qlog scheduler will consider the priority of threads
 */

#define QLOG_SCHED_POLICY SCHED_RR

#define QLOG_SCHED_MIN_PRIORITY 5
#define QLOG_SCHED_MAX_PRIORITY 85

typedef struct qlog_scheduler_s {
  struct sched_param params[IMPORTER_FUNNEL];
  pthread_attr_t attributes[IMPORTER_FUNNEL];
  int policy;
  bool balance;
} qlog_scheduler_t;

extern qlog_scheduler_t qlog_scheduler;

void qlog_scheduler_init();
void qlog_scheduler_set(pthread_t *qlog_worker, int weight_priority,
                        uint64_t idx);

#endif /* QLOG_SCHEDULER_H  */
