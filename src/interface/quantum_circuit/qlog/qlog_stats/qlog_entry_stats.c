#include <stdbool.h>
#include "qlog_entry_stats.h"

struct qlog_entry_stats_def* qlog_entry_stats_init() {
  struct qlog_entry_stats_def* qlog_entry_stats = (struct qlog_entry_stats_def*) malloc(sizeof(struct qlog_entry_stats_def));
  qlog_entry_stats->qlog_entry_stats_was_expanded = false;
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
