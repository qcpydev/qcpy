#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef QLOG_ENTRY_H
#define QLOG_ENTRY_H
#include "../../quantum_gate/quantum_gate.h"

/*
 * qlog's entries that keep track of the user
 * inputted gate's type, name, etc.
 * */

typedef struct qlog_entry_def {
  uint64_t qlog_entry_id;                            // Unique ID, each id is (qlog, qlog_entry)
  struct qlog_entry_def* qlog_entry_prev;            // prev node for the qlog
  struct qlog_entry_def* qlog_entry_next;            // next node
  struct quantum_gate_params_def* qlog_entry_params; // params for the gate associated with qlog_entry
  struct qlog_entry_stats_def* qlog_entry_stat;      // entry stats
  global_gate_name qlog_entry_gate;                  // gate name 
  global_gate_type qlog_entry_gate_type;             // gate type
  uint64_t qlog_entry_qubits;                        // bit flagsthat stores the qubits
  uint8_t qlog_entry_qubit_cnt;                      // number of qubits
  uint32_t qlog_entry_qubit_invert : 1;
} qlog_entry_def;

struct qlog_entry_def* qlog_entry_init(uint64_t id, struct qlog_entry_def* qlog_entry_prev, uint8_t *qubits, uint8_t num_qubits, int type, int gate, float16 theta, float16 phi, float16 lambda);
struct qlog_entry_def* qlog_entry_dummy_init();
uint64_t qlog_entry_init_qubit_flags(uint8_t* qlog_entry_qubits, uint8_t count);
uint8_t* qlog_entry_deconstruct_qubit_flags(struct qlog_entry_def* qlog_entry);
void qlog_entry_delete(struct qlog_entry_def *qlog_entry);
global_gate_name qlog_entry_qg_name(struct qlog_entry_def *qlog_entry); 
global_gate_type qlog_entry_qg_type(struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_name(int name, struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_type(int type, struct qlog_entry_def *qlog_entry);
void qlog_entry_dump_content(struct qlog_entry_def *qlog_entry, bool verbose);

const char* get_qlog_entry_gate(struct qlog_entry_def *qlog_entry);
const char* get_qlog_entry_gate_type(struct qlog_entry_def *qlog_entry);
bool qlog_entry_compare_entries(struct qlog_entry_def *qlog_entry, struct qlog_entry_def *qlog_entry_to_compare);


#endif // QLOG_ENTRY_H
