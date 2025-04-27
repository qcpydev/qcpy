#include "qlog_stats.h"

struct qlog_stats_def* qlog_stats_init() {
  struct qlog_stats_def* qlog_stats = (struct qlog_stats_def*) malloc(sizeof(struct qlog_stats_def));
  qlog_stats->qlog_stat_algo_cnt = 0;
  qlog_stats->qlog_stat_gate_cnt = 0;
  qlog_stats->qlog_stat_multi_cnt = 0;
  qlog_stats->qlog_stat_single_cnt = 0;
  return qlog_stats;
} 

void qlog_stats_delete(struct qlog_stats_def* qlog_stats) {
  free(qlog_stats);
  qlog_stats = NULL;
  return;
}

void qlog_stats_dump_log(struct qlog_stats_def* qlog_stats) {
  printf("Total Gate count: %d\n", qlog_stats->qlog_stat_gate_cnt);
  printf("Single Gate Count: %d\n", qlog_stats->qlog_stat_single_cnt);
  printf("Multi Gate Count: %d\n", qlog_stats->qlog_stat_multi_cnt);
  printf("Algo Gate Count: %d\n", qlog_stats->qlog_stat_algo_cnt);
  return;
}

