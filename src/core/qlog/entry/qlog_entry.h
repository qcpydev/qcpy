#include <base.h>
#include <block.h>
#include <stdbool.h>
#include <stdint.h>

#pragma once
#ifndef QLOG_ENTRY_H
#define QLOG_ENTRY_H

/*
 * qlog's entries that keep track of the user
 * inputted gate's type, name, etc.
 * */

typedef struct qlog_entry_t qlog_entry_t;
typedef uint64_t qlog_entry_id_t;

#define QLOG_ENTRY_DUMMY_ID (0);

#define QLOG_ENTRY_MIN_QUBIT(qubit_a, qubit_b)                                 \
  (qubit_a <= qubit_b ? qubit_a : qubit_b)

#define QLOG_ENTRY_MAX_QUBIT(qubit_a, qubit_b)                                 \
  (qubit_a > qubit_b ? qubit_a : qubit_b)

#define QLOG_MAX_QUBITS 64

struct qlog_entry_t {
  qlog_entry_id_t entry_id;     // Unique ID, each id is (qlog, qlog_entry)
  qlog_entry_t *prev_entry;     // prev node for the qlog
  qlog_entry_t *next_entry;     // next node
  bitmask_t qubit_bitmask;      // bitmask that stores the qubits
  bitmask_t controlled_bitmask; // bitmask for all controlled qubits
  bitpack_t controlled_bitpack; // bitpacking for controlled qubits array
  bitmask_t target_bitmask;     // bitmask for target qubits
  bitpack_t target_bitpack;     // bitpacking for target qubits array
  param_t theta;                // theta value
  param_t phi;                  // phi value
  param_t lambda;               // lambda value
  base_gate_e gate_name;        // name of the gate
  base_type_e gate_type;        // type of the gate
  qubit_t qubit_count;          // number of qubits
  qubit_t controlled_count;
  qubit_t target_count; // number of target qubits
  qubit_t max_qubit;
  qubit_t min_qubit;
  bool inverted; // (target qubit < control qubit)
};

typedef enum {
  QLOG_ENTRY_NULL,
  QLOG_ENTRY_NULL_QUBITS,
  QLOG_ENTRY_FAILED_QUBIT_FLAGS
} qlog_entry_error_e;

qlog_entry_t *qlog_entry_init(uint64_t id, block_t block);
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

typedef struct qlog_entry_init_params_s {
  qubit_t *qubits;
  qubit_t *controls;
  qubit_t *targets;
  param_t theta;
  param_t phi;
  param_t lambda;
  base_gate_e gate_name;
  base_type_e gate_type;
  qubit_t qubit_count;
  qubit_t controlled_count;
  qubit_t target_count;
  bool inverted;
} qlog_entry_init_params_t;

qlog_entry_t *
qlog_entry_init_hadamard_gate(qlog_entry_init_params_t *qlog_entry_params);

qlog_entry_t *
qlog_entry_init_t_gate(qlog_entry_init_params_t *qlog_entry_params);

qlog_entry_t *
qlog_entry_init_tdg_gate(qlog_entry_init_params_t *qlog_entry_params);

qlog_entry_t *
qlog_entry_init_cx_gate(qlog_entry_init_params_t *qlog_entry_params);

#endif // QLOG_ENTRY_H
