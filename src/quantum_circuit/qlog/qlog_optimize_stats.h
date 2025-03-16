#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct qlog_optimize_stats_def {
  uint16_t qlog_optimize_stats_gen;
  uint16_t qlog_optimize_stats_gates_del;
} qlog_optimize_stats_def;

struct qlog_optimize_stats_def* qlog_optimize_stats_init();
void qlog_optimize_stats_delete(struct qlog_optimize_stats_def* qlog_optimize_stats);
void qlog_optimize_stats_dump_log(struct qlog_optimize_stats_def* qlog_optimize_stats);
