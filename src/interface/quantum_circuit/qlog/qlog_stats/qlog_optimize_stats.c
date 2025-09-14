#include "qlog_optimize_stats.h"

struct qlog_optimize_stats_def* qlog_optimize_stats_init() {
  struct qlog_optimize_stats_def* qlog_optimize_stats = (struct qlog_optimize_stats_def*) malloc(sizeof(struct qlog_optimize_stats_def));
  return qlog_optimize_stats;
}

void qlog_optimize_stats_delete(struct qlog_optimize_stats_def* qlog_optimize_stats) {
  free(qlog_optimize_stats);
  qlog_optimize_stats = NULL;
  return;
}

void qlog_optimize_stats_dump_log(struct qlog_optimize_stats_def* qlog_optimize_stats) {
  printf("Number of passes:%d\n", qlog_optimize_stats->qlog_optimize_stats_gen);
  printf("Gates deleted:%d\n", qlog_optimize_stats->qlog_optimize_stats_gates_del);
  return;
}
