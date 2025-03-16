#include <stdint.h>
#include <stdio.h>
#include "../../quantum_gate/quantum_gate.h"

/*
 * qlog's entries that keep track of the user
 * inputted gate's type, name, etc.
 * */

typedef struct qlog_entry_def {
  uint8_t *qlog_entry_qubits;                   // int arr to 
  uint8_t qlog_entry_qubit_cnt;                 // number of qubits
  global_gate_name qlog_entry_gate;             // gate name 
  global_gate_type qlog_entry_gate_type;        // gate type
  struct quantum_gate_def* qlog_quantum_gate;   // quantum gate
  struct qlog_entry_stats_def* qlog_entry_stat; // entry stats
} qlog_entry_def;


struct qlog_entry_def* qlog_entry_init(uint8_t *qubits, uint8_t num_qubits, int type, int gate, uint8_t qlog_qubits, float16 theta, float16 phi, float16 lambda);
void qlog_entry_delete(struct qlog_entry_def *qlog_entry);
global_gate_name qlog_entry_qg_name(struct qlog_entry_def *qlog_entry); 
global_gate_type qlog_entry_qg_type(struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_name(int name, struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_type(int type, struct qlog_entry_def *qlog_entry);
void qlog_entry_dump_content(struct qlog_entry_def *qlog_entry, bool verbose);

const char* get_qlog_entry_gate(struct qlog_entry_def *qlog_entry);
const char* get_qlog_entry_gate_type(struct qlog_entry_def *qlog_entry);
bool qlog_entry_compare_entries(struct qlog_entry_def *qlog_entry, struct qlog_entry_def *qlog_entry_to_compare);
