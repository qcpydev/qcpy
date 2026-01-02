//#include "optimize/qlog_optimize.h"
//#include "stats/qlog_stats.h"
#include <assert.h>
#include <qlog_entry.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <block.h>
#include <qlog_graph.h>

#pragma once
#ifndef QLOG_H
#define QLOG_H

/*
 * qlog is designed to keep track of user's inputs for optimization and
 * organization.
 */

typedef struct qlog_t {
  qlog_entry_t *entries;               // qlog entries
  qlog_entry_t *last_entry;            // qlog last inserted item
  qlog_graph_t* graph;                 // quantum circuit layout
  uint64_t id;                         // id from the qlog_register
  uint16_t qubit_count;                // number of qubits
  uint16_t entry_count;                // number of entries in qlog
} qlog_t;

typedef enum {
  QLOG_NULL,
  QLOG_BAD_ENTRY,
  QLOG_CORRUPT,
  QLOG_OPTIMIZE_FAILED,
  QLOG_BAD_SIZING,
  QLOG_DELETE_FAILED
} qlog_error_e;

qlog_t* qlog_init(uint8_t qubits);
void qlog_delete(qlog_t *qlog);
void qlog_append(qlog_t *qlog, block_t block);
bool qlog_append_entry(qlog_t *qlog, qlog_entry_t *qlog_entry);
void qlog_clear();
void qlog_dump_content(qlog_t *qlog, bool verbose);
bool qlog_compare(qlog_t *qlog, qlog_t *qlog_q);
qlog_t *qlog_combine(qlog_t *qlog, qlog_t *to_combine);

#endif
