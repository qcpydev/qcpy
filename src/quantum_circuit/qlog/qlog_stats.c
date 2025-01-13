#include "qlog_stats.h"

struct qlog_stats_def* qlog_stats_init() {
  struct qlog_stats_def* qlog_stats = {0};
  return qlog_stats;
} 

void qlog_stats_delete(struct qlog_stats_def* qlog_stats) {
  free(qlog_stats);
  qlog_stats = NULL;
  return;
}

void qlog_stats_dump_log(struct qlog_stats_def* qlog_stats) {
  printf("Total Gate count:%d\n", qlog_stats->qlog_stat_gate_cnt);
  printf("Single Gate Count: %d\n", qlog_stats->qlog_stat_single_cnt);
  printf("Multi Gate Count: %d\n", qlog_stats->qlog_stat_multi_cnt);
  printf("Algo Gate Count: %d\n", qlog_stats->qlog_stat_algo_cnt);
  return;
}

struct qlog_entry_stats_def* qlog_entry_stats_init() {
  struct qlog_entry_stats_def* qlog_entry_stats = {0};
  return qlog_entry_stats;
}

void qlog_entry_stats_delete(struct qlog_entry_stats_def* qlog_entry_stats) {
  free(qlog_entry_stats);
  qlog_entry_stats = NULL;
  return;
}

void qlog_entry_stats_dump_log(struct qlog_entry_stats_def* qlog_entry_stats) {
  printf("Was expanded?: %d\n", qlog_entry_stats->qlog_entry_stats_was_expanded);
  return;
}

struct qlog_optimize_stats_def* qlog_optimize_stats_init() {
  struct qlog_optimize_stats_def* qlog_optimize_stats = {0};
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
