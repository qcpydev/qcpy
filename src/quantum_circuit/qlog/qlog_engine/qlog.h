#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "qlog_stats.h"
/*
 * In port functions/types:
 * */
typedef enum {
  QLOG_ENTRY_GATE_IDENTITY,
  QLOG_ENTRY_GATE_HADAMARD,
  QLOG_ENTRY_GATE_PAULIX,
  QLOG_ENTRY_GATE_PAULIY,
  QLOG_ENTRY_GATE_PAULIZ,
  QLOG_ENTRY_GATE_PHASE,
  QLOG_ENTRY_GATE_S,
  QLOG_ENTRY_GATE_SDG,
  QLOG_ENTRY_GATE_T,
  QLOG_ENTRY_GATE_TDG,
  QLOG_ENTRY_GATE_RZ,
  QLOG_ENTRY_GATE_RY,
  QLOG_ENTRY_GATE_RX,
  QLOG_ENTRY_GATE_SX,
  QLOG_ENTRY_GATE_SXDG,
  QLOG_ENTRY_GATE_U,
  QLOG_ENTRY_GATE_CX,
  QLOG_ENTRY_GATE_CH,
  QLOG_ENTRY_GATE_CY,
  QLOG_ENTRY_GATE_CZ,
  QLOG_ENTRY_GATE_CRX,
  QLOG_ENTRY_GATE_CRY,
  QLOG_ENTRY_GATE_CRZ,
  QLOG_ENTRY_GATE_CR1,
  QLOG_ENTRY_GATE_CCX,
  QLOG_ENTRY_GATE_QFT,
  QLOG_ENTRY_GATE_RCCX,
  QLOG_ENTRY_GATE_RC3X,
  QLOG_ENTRY_GATE_SWAP,
  QLOG_ENTRY_GATE_RXX,
  QLOG_ENTRY_GATE_RZZ,
  QLOG_ENTRY_GATE_CUSTOM,
  QLOG_ENTRY_GATE_CUSTOMCONTROLLED,
  QLOG_ENTRY_GATE_MULTI,
  QLOG_ENTRY_GATE_CUSTOMBLOCK,
  QLOG_ENTRY_GATE_CUSTOMALGORITHM
} qlog_entry_gate;

typedef enum {
  QLOG_ENTRY_TYPE_SINGLE,
  QLOG_ENTRY_TYPE_CONTROLLED,
  QLOG_ENTRY_TYPE_MULTI,
  QLOG_ENTRY_TYPE_BLOCK,
  QLOG_ENTRY_TYPE_ALGORITHM
} qlog_entry_type;

typedef enum {
  QLOG_APPEND_SUCCESS,
  QLOG_APPEND_FULL,
  QLOG_APPEND_ERROR
} qlog_append_res;

typedef struct complex_64 {
  float real;
  float imag;
} complex_64;

typedef struct gate_matrix_def {
  uint8_t gate_matrix_size;
  struct complex_64 **gate_matrix_gate;
} gate_matrix_def;

/*
 * qlog is designed to keep track of user's inputs for optimization and organization.
 * */
typedef struct qlog_def {
  uint16_t qlog_qubit_cnt; // number of qubits inside of the 
  uint16_t qlog_size; // number of entries in qlog
  struct qlog_entry_def **qlog_entries; // list of qlog_entry
  struct qlog_stats_def qlog_stat; // qlog stats
} qlog_type;

/*
 * qlog's entries that keep track of the user inputted gate's type, name, etc.
 * */

typedef struct qlog_entry_def {
  uint8_t *qlog_entry_qubits; // int arr to 
  uint8_t qlog_entry_qubit_cnt; // number of qubits
  qlog_entry_gate qlog_entry_gate; // gate name 
  qlog_entry_type qlog_entry_gate_type; // gate type
  struct gate_matrix_def gate_matrix; // gate matrix content
  struct qlog_entry_stats_def qlog_entry_stat; // entry stats
} qlog_entry_def;


struct qlog_def* qlog_init(uint8_t qubits);
void qlog_delete(struct qlog_def *qlog);
uint16_t qlog_size(struct qlog_def *qlog);
qlog_append_res qlog_append(struct qlog_def *qlog, uint8_t *qubits, uint8_t num_qubits, int type, int gate);
void qlog_print_content(struct qlog_def *qlog);
void qlog_clear();
void qlog_dump_content(struct qlog_def *qlog, bool verbose); // need to implement

/*
 * Offshore functions/types
 * */

struct qlog_entry_def* qlog_entry_init(uint8_t *qubits, uint8_t num_qubits, int type, int gate, uint8_t qlog_qubits);
void qlog_entry_delete(struct qlog_entry_def *qlog_entry);
qlog_entry_gate qlog_entry_qg_name(struct qlog_entry_def *qlog_entry); 
qlog_entry_type qlog_entry_qg_type(struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_name(int name, struct qlog_entry_def *qlog_entry);
bool qlog_entry_set_qg_type(int type, struct qlog_entry_def *qlog_entry);
void qlog_entry_dump_content(struct qlog_entry_def *qlog_entry, bool verbose);

struct gate_matrix_def* gate_matrix_init(uint8_t size, complex_64 **gate_matrix_gate);
void gate_matrix_delete(struct gate_matrix_def *gate_matrix);
uint8_t gate_matrix_get_size(struct gate_matrix_def *gate_matrix);
void gate_matrix_dump_content(struct gate_matrix_def *gate_matrix);

const char* get_qlog_entry_gate(struct qlog_entry_def *qlog_entry);
const char* get_qlog_entry_gate_type(struct qlog_entry_def *qlog_entry);
