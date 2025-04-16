#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "qlog_entry.h"

#pragma once
#ifndef QLOG_H
#define QLOG_H

/*
 * In port functions/types:
 * */

typedef enum {
  QLOG_APPEND_SUCCESS,
  QLOG_APPEND_FULL,
  QLOG_APPEND_ERROR
} qlog_append_res;

/*
 * qlog is designed to keep track of user's inputs for optimization and organization.
 *
 * */

typedef struct qlog_def {
  struct qlog_entry_def* qlog_entry_list;
  struct qlog_entry_def* qlog_entry_last;
  uint16_t qlog_qubit_cnt;              // number of qubits inside of the 
  uint16_t qlog_size;                   // number of entries in qlog
  struct qlog_stats_def* qlog_stat;     // qlog stats
} qlog_type;

struct qlog_def* qlog_init(uint8_t qubits);
void qlog_delete(struct qlog_def *qlog);
uint16_t qlog_size(struct qlog_def *qlog);
qlog_append_res qlog_append(struct qlog_def *qlog, uint8_t *qubits, uint8_t num_qubits, int type, int gate, float16 theta, float16 phi, float16 lambda);
qlog_append_res qlog_append_entry(struct qlog_def *qlog, struct qlog_entry_def *qlog_entry);
void qlog_print_content(struct qlog_def *qlog);
void qlog_clear();
void qlog_dump_content(struct qlog_def *qlog, bool verbose);
bool qlog_compare_qlogs(struct qlog_def *qlog_t, struct qlog_def *qlog_q);
struct qlog_def* qlog_combine_qlogs(struct qlog_def *qlog_t, struct qlog_def *qlog_q);
char** qlog_get_gate_names(struct qlog_def *qlog);
char** qlog_get_gate_types(struct qlog_def *qlog);
uint8_t** qlog_get_gate_qubits(struct qlog_def *qlog);
uint8_t* qlog_get_entry_sizes(struct qlog_def *qlog);
#endif // QLOG_H
