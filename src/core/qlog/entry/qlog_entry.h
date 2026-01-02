#include "../../base/base.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <block.h>

#pragma once
#ifndef QLOG_ENTRY_H
#define QLOG_ENTRY_H

/*
 * qlog's entries that keep track of the user
 * inputted gate's type, name, etc.
 * */

typedef struct qlog_entry_t qlog_entry_t;

struct qlog_entry_t {
  uint64_t entry_id;                 // Unique ID, each id is (qlog, qlog_entry)
  qlog_entry_t *prev_entry;          // prev node for the qlog
  qlog_entry_t *next_entry;          // next node
  uint64_t qubit_bitmask;            // bitmask that stores the qubits
  uint64_t controlled_bitmask;       // bitmask for all controlled qubits
  uint64_t controlled_bitpack;       // bitpacking for controlled qubits array
  uint64_t target_bitmask;           // bitmask for target qubits
  uint64_t target_bitpack;           // bitpacking for target qubits array
  float theta;                       // theta value
  float phi;                         // phi value
  float lambda;                      // lambda value
  base_gate_e gate_name;             // name of the gate
  base_type_e gate_type;             // type of the gate
  uint8_t qubit_count;               // number of qubits
  uint8_t controlled_count;          // number of controlled qubits
  uint8_t target_count;              // number of target qubits
  bool inverted;                     // (target qubit < control qubit)
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
uint16_t* qlog_entry_unpack(uint8_t qubits, uint64_t bitpack);

#endif // QLOG_ENTRY_H
