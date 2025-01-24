#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "qlog_stats.h"
#include "../../quantum_gate/quantum_gate.h"

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
  uint16_t qlog_qubit_cnt; // number of qubits inside of the 
  uint16_t qlog_size; // number of entries in qlog
  struct qlog_entry_def **qlog_entries; // list of qlog_entry
  struct qlog_stats_def* qlog_stat; // qlog stats
} qlog_type;

/*
 * qlog's entries that keep track of the user inputted gate's type, name, etc.
 * */

typedef struct qlog_entry_def {
  uint8_t *qlog_entry_qubits; // int arr to 
  uint8_t qlog_entry_qubit_cnt; // number of qubits
  global_gate_name qlog_entry_gate; // gate name 
  global_gate_type qlog_entry_gate_type; // gate type
  struct quantum_gate_def* qlog_quantum_gate; // quantum gate
  struct qlog_entry_stats_def* qlog_entry_stat; // entry stats
} qlog_entry_def;

struct qlog_def* qlog_init(uint8_t qubits);
void qlog_delete(struct qlog_def *qlog);
uint16_t qlog_size(struct qlog_def *qlog);
qlog_append_res qlog_append(struct qlog_def *qlog, uint8_t *qubits, uint8_t num_qubits, int type, int gate);
void qlog_print_content(struct qlog_def *qlog);
void qlog_clear();
void qlog_dump_content(struct qlog_def *qlog, bool verbose);

/*
 * Offshore functions/types
 * */

struct qlog_entry_def* qlog_entry_init(uint8_t *qubits, uint8_t num_qubits, int type, int gate, uint8_t qlog_qubits);
void qlog_entry_delete(struct qlog_entry_def *qlog_entry);
global_gate_name qlog_entry_qg_name(struct qlog_entry_def *qlog_entry); 
global_gate_type qlog_entry_qg_type(struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_name(int name, struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_type(int type, struct qlog_entry_def *qlog_entry);
void qlog_entry_dump_content(struct qlog_entry_def *qlog_entry, bool verbose);

const char* get_qlog_entry_gate(struct qlog_entry_def *qlog_entry);
const char* get_qlog_entry_gate_type(struct qlog_entry_def *qlog_entry);
