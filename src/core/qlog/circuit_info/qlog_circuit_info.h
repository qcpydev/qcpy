#include "../qlog.h"

typedef struct qlog_circuit_info {
  char **names;
  char **types;
  uint8_t **gate_names;
  uint8_t *sizes;
} qlog_circuit_info;

char **qlog_get_gate_names(struct qlog_def *qlog);
char **qlog_get_gate_types(struct qlog_def *qlog);
uint8_t **qlog_get_gate_qubits(struct qlog_def *qlog);
uint8_t *qlog_get_entry_sizes(struct qlog_def *qlog);
