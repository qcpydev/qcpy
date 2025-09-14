#include "../../base/base.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#pragma once
#ifndef QLOG_ENTRY_H
#define QLOG_ENTRY_H

/*
 * qlog's entries that keep track of the user
 * inputted gate's type, name, etc.
 * */

typedef struct qlog_entry_t qlog_entry_t;

struct qlog_entry_t {
  uint64_t entry_id;        // Unique ID, each id is (qlog, qlog_entry)
  qlog_entry_t *prev_entry; // prev node for the qlog
  qlog_entry_t *next_entry; // next node
  struct gate_params_t
      *gate_params; // params for the gate associated with qlog_entry
  struct qlog_entry_stats_t *stats; // entry stats
  base_gate_e gate_name;            // name of the gate
  base_type_e gate_type;            // type of the gate
  uint64_t qubits;                  // bit flags that stores the qubits
  uint8_t qubit_count;              // number of qubits
  bool inverted; // flipped gate entry (target qubit < control qubit)
};

typedef enum {
  QLOG_ENTRY_NULL,
  QLOG_ENTRY_NULL_QUBITS,
  QLOG_ENTRY_FAILED_QUBIT_FLAGS
} qlog_entry_error_e;

qlog_entry_t *qlog_entry_init(uint64_t id, qlog_entry_t *qlog_entry_prev,
                              uint8_t *qubits, uint8_t num_qubits, int type,
                              int gate, float theta, float phi, float lambda);
uint64_t qlog_entry_qubit_set_flags(uint8_t *qubits, uint8_t count);
uint8_t *qlog_entry_deconstruct_qubits(qlog_entry_t *qlog_entry);
void qlog_entry_delete(qlog_entry_t *qlog_entry);
base_gate_e qlog_entry_qg_name(qlog_entry_t *qlog_entry);
base_type_e qlog_entry_qg_type(qlog_entry_t *qlog_entry);
bool qlog_entry_set_qg_name(int name, qlog_entry_t *qlog_entry);
bool qlog_entry_set_qg_type(int type, qlog_entry_t *qlog_entry);
void qlog_entry_dump_content(qlog_entry_t *qlog_entry, bool verbose);
qlog_entry_t *qlog_entry_duplicates_to_clean(qlog_entry_t *qlog_entry);
const char *get_qlog_entry_gate(qlog_entry_t *qlog_entry);
const char *get_qlog_entry_gate_type(qlog_entry_t *qlog_entry);
bool qlog_entry_compare(qlog_entry_t *qlog_entry, qlog_entry_t *to_compare);

#endif // QLOG_ENTRY_H
