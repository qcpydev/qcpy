#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "qlog_entry.h"
#include "qlog_optimize_graph.h"
#include "qlog.h"

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

typedef enum {
  QLOG_TRIGGER_OPT_IDENTITY_GATES,
  QLOG_TRIGGER_OPT_SAME_GATES,
  QLOG_TRIGGER_OPT_USELESS_CNOT,
} qlog_trigger_optimizers_types;

typedef struct qlog_trigger_optimizers_def {
  uint64_t qlog_trigger_optimizers_gate_cnt;
  qlog_trigger_optimizers_types qlog_trigger_optimizers_type;
  struct qlog_entry_def* qlog_trigger_optimizers_entry_list;
  struct qlog_trigger_optimize_def* qlog_trigger_optimizers_parent;
  uint16_t qlog_trigger_optimizers_threshold;
} qlog_trigger_optimizers_def; 

typedef struct qlog_trigger_optimize_def {
  uint64_t qlog_trigger_optimize_gate_cnt;
  uint8_t qlog_trigger_optimize_qubit_cnt;
  struct qlog_optimize_graph_def* qlog_trigger_optimize_graph;
  struct qlog_trigger_optimizers_def** qlog_trigger_optimizers;
  struct qlog_def* qlog_trigger_optimize_parent;
} qlog_trigger_optimize_def;

struct qlog_trigger_optimize_def qlog_trigger_optimize_init(struct qlog_def* qlog);
void qlog_trigger_optimize_delete(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
void qlog_trigger_optimize_append_entry(struct qlog_entry_def* qlog_entry, struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_lock_if_met(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_lock_graph(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_can_do_work(struct qlog_trigger_optimize_def* qlog_trigger_optimize);

struct qlog_trigger_optimizers_def qlog_trigger_optimizers_init(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
void qlog_trigger_optimizers_delete(struct qlog_trigger_optimzers_def* qlog_trigger_optimizer);
bool qlog_trigger_optimizers_threshold_met(struct qlog_trigger_optimizers_def* qlog_trigger_optimizer);
void qlog_trigger_optimizers_append_entry(struct qlog_entry_def* qlog_entry, struct qlog_trigger_optimizer);
bool qlog_trigger_optimize_can_do_work(struct qlog_trigger_optimize_def* qlog_trigger_optimize);

#endif // QLOG_TRIGGER_OPTIMIZE_H
