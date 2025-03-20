#include <stdio.h>
#include <stdlib.h>

typedef struct qlog_entry_stats_def {
  bool qlog_entry_stats_was_expanded; 
} qlog_entry_stats;

struct qlog_entry_stats_def* qlog_entry_stats_init();
void qlog_entry_stats_delete(struct qlog_entry_stats_def* qlog_entry_stats);
void qlog_entry_stats_dump_log(struct qlog_entry_stats_def* qlog_stats);
