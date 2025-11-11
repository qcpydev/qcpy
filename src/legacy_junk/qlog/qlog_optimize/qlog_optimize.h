#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "../qlog.h"
#include "../qlog_stats/qlog_optimize_stats.h"

#pragma once
#ifndef QLOG_OPTIMIZE_H

/*
 * qlog optimize
 * This code is the basement dweler of the already basement dweler of qlog
 * This code calls systems that tracks user inputs and determines if unique thresholds
 * can be met to optimize async. qlog_def should be the only segment of code that should
 * interact with this and send off a optimized node graph to then have quack format and
 * do what it wants with this optimized graph format.
 * When qlog asks for this graph, using qlog_optimize_force will await for the async
 * process to finish, flush the backlog, then commit to an awaited qlog_optimize_trigger.
 * Finally, qlog_optimize will return the graph for quack to process.
 * Note: qlog_optimize_graph is simply a pointer that is a nice to have to track for future
 * enhancements, qlog_optimize_trigger will mainly be tracking and updating this as needed
 * */

typedef struct qlog_optimize_def {
  struct qlog_optimize_stats_def* qlog_optimize_stats;      // stats for debugging
  struct qlog_def* qlog_optimize_parent;                    // ptr to qlog_def
  struct qlog_trigger_optimize_def* qlog_optimize_trigger;  // sub lists of entries to trigger 
  struct qlog_entry_def* qlog_optimize_list;                // list of entries that can/will be optimized
  struct qlog_entry_def* qlog_optimize_last;                // last entry in the list 
  struct qlog_entry_def* qlog_optimize_backlog;             // ptr to qlog item that needs processing during async lock
  uint64_t qlog_optimize_backlog_size;                      // qlog_optimize_backsize
  uint8_t qlog_optimize_size;                               // number of qubits
  volatile int qlog_optimize_is_running : 1;                // lock qlog_optimize_trigger from taking in entries
} qlog_optimize_def;

struct qlog_graph_def* qlog_optimize_force(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_append(struct qlog_optimize_def* qlog_optimize, struct qlog_entry_def* qlog_entry);
void qlog_optimize_dump_log(struct qlog_optimize_def* qlog_optimize, bool verbose);
struct qlog_optimize_def* qlog_optimize_init(struct qlog_def* qlog);
void qlog_optimize_delete(struct qlog_optimize_def* qlog_optimize);

bool qlog_optimize_stop(struct qlog_optimize_def* qlog_optimize);
bool qlog_optimize_run(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_flush_backlog(struct qlog_optimize_def* qlog_optimize);

#endif // QLOG_OPTIMIZE_H

