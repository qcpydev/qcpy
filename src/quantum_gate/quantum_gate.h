#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../global/global.h"

typedef struct quantum_gate_def {
  global_gate_name quantum_gate_name;
  global_gate_type quantum_gate_type;
  struct global_matrix_def quantum_gate_matrix;
  uint16_t rows;
  uint16_t cols;
  uint16_t size;
} quantum_gate_def;


quantum_gate_def* quantum_gate_init(uint16_t rows, uint16_t cols, uint16_t size, global_gate_name gate_name, global_gate_type gate_type);
void quantum_gate_delete(struct quantum_gate_def *quantum_gate);
global_matrix_def* quantum_gate_export();
void quantum_gate_import();
