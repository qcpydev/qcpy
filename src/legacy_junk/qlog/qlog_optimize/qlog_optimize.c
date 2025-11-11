#include "qlog_optimize.h"
#include "qlog_optimize_graph.h"
#include "qlog_trigger_optimize.h"

struct qlog_optimize_def* qlog_optimize_init(struct qlog_def* qlog) {
  if (!qlog) {
    return NULL;
  }

  struct qlog_optimize_def* qlog_optimize = (struct qlog_optimize_def*) malloc(sizeof(qlog_optimize_def));

  qlog_optimize->qlog_optimize_parent = qlog;
  qlog_optimize->qlog_optimize_size = qlog->qlog_size;
  qlog_optimize->qlog_optimize_stats = qlog_optimize_stats_init();
  qlog_optimize->qlog_optimize_trigger = qlog_trigger_optimize_init(qlog);
  struct qlog_entry_def* qlog_optimize_list = (struct qlog_entry_def*)malloc(sizeof(struct qlog_entry_def));
  qlog_optimize->qlog_optimize_list = qlog_optimize_list;
  qlog_optimize->qlog_optimize_last = qlog_optimize_list;

  // qlog_optimize_append_what to do:
  // copy entries if exist from qlog to qlog_optimize
  return qlog_optimize;
}

void qlog_optimize_delete(struct qlog_optimize_def* qlog_optimize) {
  if (!qlog_optimize) {
    return;
  }

  qlog_optimize->qlog_optimize_parent = NULL;
  // call for free func for qlog_optimize->qlog_optimize_stats
  if (qlog_optimize->qlog_optimize_stats) {
    return;
  }
  // call for free func for qlog_optimize->qlog_optimize_trigger
  if (qlog_optimize->qlog_optimize_trigger) {
    return;
  }
}

void qlog_optimize_dump_log(struct qlog_optimize_def* qlog_optimize, bool verbose) {
  if (!qlog_optimize) {
    return;
  }
  // print stuff here
}

bool qlog_optimize_running(struct qlog_optimize_def* qlog_optimize) {
  if (!qlog_optimize) {
    return false;
  }

  return qlog_optimize->qlog_optimize_is_running;
}


struct qlog_graph_def* qlog_optimize_force(struct qlog_optimize_def* qlog_optimize) {
  if (!qlog_optimize) {
    return NULL;
  }

  qlog_optimize_run(qlog_optimize);

  // "await" for the code to complete
  while (qlog_optimize_running(qlog_optimize));
  // OR IF THESHOLD FOR TIME COMPLETION in case of hang lets call it 10sec
  return NULL;
}

void qlog_optimize_append(struct qlog_optimize_def* qlog_optimize, struct qlog_entry_def* qlog_entry) {
  if (!qlog_optimize) {
    return;
  }

  if (!qlog_entry) {
    return;
  }

  struct qlog_entry_def* qlog_entry_cpy;
  memcpy(qlog_entry_cpy, qlog_entry, sizeof(struct qlog_entry_def));

  if (!qlog_entry_cpy) {
    return;
  }
  qlog_entry_cpy->qlog_entry_prev = qlog_optimize->qlog_optimize_last;
  qlog_optimize->qlog_optimize_last->qlog_entry_next = qlog_entry_cpy;
  qlog_optimize->qlog_optimize_last = qlog_entry_cpy;
  ++qlog_optimize->qlog_optimize_size;

  if (qlog_optimize_running(qlog_optimize)) {
    ++qlog_optimize->qlog_optimize_size; 
    if (!qlog_optimize->qlog_optimize_backlog) {
      qlog_optimize->qlog_optimize_backlog = qlog_entry;
    }
  }

  qlog_trigger_optimize_append_entry(qlog_entry, qlog_optimize->qlog_optimize_trigger);
}

bool qlog_optimize_stop(struct qlog_optimize_def* qlog_optimize) {
  if (!qlog_optimize) {
    return false;
  }

  // tell qlog_trigger_optimize to stop doing whatever it is doing

  while (qlog_optimize_running(qlog_optimize));

  return true;
}

bool qlog_optimize_run(struct qlog_optimize_def* qlog_optimize) {

  if (!qlog_optimize) {
    return false;    
  }

  qlog_trigger_optimize(qlog_optimize->qlog_optimize_trigger);

  return qlog_optimize_running(qlog_optimize);
}

