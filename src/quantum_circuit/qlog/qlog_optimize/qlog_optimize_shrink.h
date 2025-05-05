#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "qlog_optimize.h"
#include "qlog_optimize_graph.h"

/*
 * qlog optimize shrink
 * This contains a list of atomic shrink operations that qlog_optimize can use.
 * Depending on the sequence of execution, when doing a force optimization,
 * qlog optimize expand will expand the gates, in which these set of optimizations
 * will run and minimize as much as it can
 * No shrink process should purely rely on another shrink message. 
 * All of these shrinks should only run inside of the qlog_optimize thread.
 * */

#pragma once
#ifndef QLOG_OPTIMIZE_SHRINK_H
#define QLOG_OPTIMIZE_SHRINK_H

typedef enum {
  QLOG_TRIGGER_SHRINK_UNUSED_BLOCK,
  QLOG_TRIGGER_SHRINK_UNUSED_MULTICONTROLLED,
  QLOG_TRIGGER_SHRINK_HADAMARD_CONTROLLED,
  QLOG_TRIGGER_SHRINK_UNUSED_CONTROLLED,
  QLOG_TRIGGER_SHRINK_DUPLICATE_GATES,
  QLOG_TRIGGER_SHRINK_IDENTITY_GATES,
  QLOG_TRIGGER_SHRINK_MAX
} qlog_trigger_optimize_shrink_types;

typedef struct qlog_trigger_optimize_sub_def {
  uint64_t qlog_trigger_optimize_sub_gate_cnt;                                                             // gate cnt
  struct qlog_entry_def* qlog_trigger_optimize_sub_entry_list;                                             // sub-lists taking account for patterns
  struct qlog_entry_def* qlog_trigger_optimize_sub_last;                                                   // end of the list 
  qlog_trigger_optimize_shrink_types qlog_trigger_optimize_sub_type;                                       // type of optimization
  bool (*qlog_trigger_optimize_sub_append)(struct qlog_trigger_optimize_sub_def*,                          // ptr to function to do work
                                           struct qlog_graph_def*,
                                           struct qlog_entry_def*); 
  uint16_t qlog_trigger_optimize_sub_pattern_cnt;                                                          // count of found patterns
  uint16_t qlog_trigger_optimize_sub_threshold_min;                                                        // optimizations can happen threshold
  uint16_t qlog_trigger_optimize_sub_threshold_max;                                                        // optimizations must happen threshold
  uint16_t qlog_trigger_optimize_sub_expand : 1;                                                           // determinant if optimize_sub type can expand
} qlog_trigger_optimize_sub_def;

typedef bool (*qlog_trigger_optimize_sub_trigger)(struct qlog_trigger_optimize_sub_def*, struct qlog_graph_def*, struct qlog_entry_def*);
 
struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_base(qlog_trigger_optimize_sub_trigger trigger_shrink_func,
                                                                          qlog_trigger_optimize_shrink_types shrink_type, 
                                                                          uint16_t trigger_threshold_min,
                                                                          uint16_t trigger_threshold_max);

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_block(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_optimize_shrink_trigger_unused_block(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry);

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_multicontrolled(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_optimize_shrink_trigger_unused_multicontrolled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry);

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_hadamard_controlled(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_optimize_shrink_trigger_hadamard_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry);

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_controlled(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_optimize_shrink_trigger_unused_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry);

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_duplicate_gates(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_optimize_shrink_trigger_duplicate_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry);

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_identity_gates(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_optimize_shrink_trigger_identity_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry);

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
void qlog_trigger_optimize_sub_delete(struct qlog_trigger_optimize_sub_def* qlog_trig_sub);
void qlog_trigger_optimize_sub_dump_log(struct qlog_trigger_optimize_sub_def* qlog_trig_sub);

#endif // QLOG_OPTIMIZE_SHRINK_H
