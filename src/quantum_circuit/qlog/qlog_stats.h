#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef QLOG_STATS_H
#define QLOGSTATS_H

#define STAT_VALUE_INC(stats, stat_field) (++stats->stat_field)
#define STAT_VALUE_INC_BY(stats, stat_field, value) (stats->stat_field += value)
#define STAT_VALUE_DEC(stats, stat_field) (--stats->stat_field)
#define STAT_VALUE_DEC_BY(stats, stat_field, value) (stats->stat_field -= value)

typedef struct qlog_stats_def {
  uint16_t qlog_stat_gate_cnt;
  uint16_t qlog_stat_single_cnt;
  uint16_t qlog_stat_multi_cnt;
  uint16_t qlog_stat_algo_cnt;
} qlog_stats;

typedef struct qlog_entry_stats_def {
  bool qlog_entry_stats_was_expanded; 
} qlog_entry_stats;


typedef struct qlog_optimize_stats_def {
  uint16_t qlog_optimize_stats_gen;
  uint16_t qlog_optimize_stats_gates_del;
} qlog_optimize_stats_def;

struct qlog_stats_def* qlog_stats_init();
void qlog_stats_delete(struct qlog_stats_def* qlog_stat);
void qlog_stats_dump_log(struct qlog_stats_def* qlog_stats);


struct qlog_entry_stats_def* qlog_entry_stats_init();
void qlog_entry_stats_delete(struct qlog_entry_stats_def* qlog_entry_stats);
void qlog_entry_stats_dump_log(struct qlog_entry_stats_def* qlog_stats);

struct qlog_optimize_stats_def* qlog_optimize_stats_init();
void qlog_optimize_stats_delete(struct qlog_optimize_stats_def* qlog_optimize_stats);
void qlog_optimize_stats_dump_log(struct qlog_optimize_stats_def* qlog_optimize_stats);

#endif 
