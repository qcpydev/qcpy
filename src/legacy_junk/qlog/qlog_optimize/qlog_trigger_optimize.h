#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../qlog_entry.h"
#include "../qlog.h"
#include "qlog_optimize_graph.h"
#include "qlog_optimize_shrink.h"
#include "qlog_optimize_expand.h"

/*
 * qlog_trigger_optimize
 * This is a series of queues that track qlog_entries that meet certain criteria.
 * These criterias are defined uniquely in their own qlog_trigger_optimizers struct.
 * These optimizers are stored in qlog_trigger_optimize_def, which will send a trigger
 * message letting the struct know to:
 * 1. Lock the qlog_optimize series which is stored in each qlog_def
 * 2. If already locked and used by another qlog_optimize_trigger previously, append it to the queue of work needing to be done.
 * 3. These entries need to also be kept track of when other optimizers with higher priority remove or add more gates, so such
 *    work that needs to be done can take accounted for if less or more work is needed on the qlog_optimize.
 * */

#pragma once
#ifndef QLOG_TRIGGER_OPTIMIZE_H
#define QLOG_TRIGGER_OPTIMIZE_H

typedef struct qlog_trigger_optimize_def {
  uint64_t qlog_trigger_optimize_gate_cnt;                            // gate count of current qlog_optimize
  struct qlog_graph_def* qlog_trigger_optimize_graph;                 // qlog optimize graph
  struct qlog_trigger_optimize_sub_def** qlog_trigger_optimize_subs;  // optimize sub lists
  struct qlog_def* qlog_trigger_optimize_parent;                      // parent of this qlog_trigger_optimize
  uint8_t qlog_trigger_optimize_qubit_cnt;                            // qubit count of this optimizer
  struct qlog_entry_def* qlog_trigger_optimize_expand_queue;          // queue of items to expand
  struct qlog_entry_def* qlog_trigger_optimize_expand_queue_last;     // last entered item in expand queue
} qlog_trigger_optimize_def;

struct qlog_trigger_optimize_def* qlog_trigger_optimize_init(struct qlog_def* qlog);
bool qlog_trigger_optimize(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
void qlog_trigger_optimize_delete(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
void qlog_trigger_optimize_dump_log(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
void qlog_trigger_optimize_append_entry(struct qlog_entry_def* qlog_entry, struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_secure(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_secure_graph(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_can_do_min_work(struct qlog_trigger_optimize_def* qlog_trigger_optimize);

#endif // QLOG_TRIGGER_OPTIMIZE_H
