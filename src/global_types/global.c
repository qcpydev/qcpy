#include "global.h"

const char *global_gate_names[] = {
  [GLOBAL_GATE_IDENTITY] = "IDENTITY",
  [GLOBAL_GATE_HADAMARD] = "HADAMARD",
  [GLOBAL_GATE_PAULIX] = "PAULIX",
  [GLOBAL_GATE_PAULIY] = "PAULIY",
  [GLOBAL_GATE_PAULIZ] = "PAULIZ",
  [GLOBAL_GATE_PHASE] = "PHASE",
  [GLOBAL_GATE_S] = "S",
  [GLOBAL_GATE_SDG] = "SDG",
  [GLOBAL_GATE_T] = "T",
  [GLOBAL_GATE_TDG] = "TDG",
  [GLOBAL_GATE_RZ] = "RZ",
  [GLOBAL_GATE_RY] = "RY",
  [GLOBAL_GATE_RX] = "RX",
  [GLOBAL_GATE_SX] = "SX",
  [GLOBAL_GATE_SXDG] = "SXDG",
  [GLOBAL_GATE_U] = "U",
  [GLOBAL_GATE_CX] = "CX",
  [GLOBAL_GATE_CH] = "CH",
  [GLOBAL_GATE_CY] = "CY",
  [GLOBAL_GATE_CZ] = "CZ",
  [GLOBAL_GATE_CRX] = "CRX",
  [GLOBAL_GATE_CRY] = "CRY",
  [GLOBAL_GATE_CRZ] = "CRZ",
  [GLOBAL_GATE_CR1] = "CR1",
  [GLOBAL_GATE_CCX] = "CCX",
  [GLOBAL_GATE_QFT] = "QFT",
  [GLOBAL_GATE_RCCX] = "RCCX",
  [GLOBAL_GATE_RC3X] = "RC3X",
  [GLOBAL_GATE_SWAP] = "SWAP",
  [GLOBAL_GATE_RXX] = "RXX",
  [GLOBAL_GATE_RZZ] = "RZZ",
  [GLOBAL_GATE_CUSTOM] = "CUSTOM",
  [GLOBAL_GATE_CUSTOMCONTROLLED] = "CUSTOMCONTROLLED",
  [GLOBAL_GATE_MULTI] = "MULTI",
  [GLOBAL_GATE_CUSTOMBLOCK] = "CUSTOMBLOCK",
  [GLOBAL_GATE_CUSTOMALGORITHM] = "CUSTOMALGORITHM",
};

const char *global_gate_types[] = {
  [GLOBAL_TYPE_SINGLE] = "SINGLE",
  [GLOBAL_TYPE_CONTROLLED] = "CONTROLLED",
  [GLOBAL_TYPE_MULTI] = "MULTI",
  [GLOBAL_TYPE_BLOCK] = "BLOCK",
  [GLOBAL_TYPE_ALGORITHM] = "ALGORITHM",
};

global_matrix_del_res global_matrix_delete(struct global_matrix_def **gate_matrix, uint16_t size) {
  if (!gate_matrix) {
    return GLOBAL_MATRIX_DEL_BAD_VALUE_ERR;
  }
  for (uint16_t i = 0; i < size; ++i) {
      free(gate_matrix[i]);
      gate_matrix[i] = NULL; 
  }
  free(gate_matrix);
  gate_matrix = NULL;
  return GLOBAL_MATRIX_DEL_SUCCESS; 
}

global_matrix_def** global_matrix_alloc(int size) {
  global_matrix_def **matrix = (global_matrix_def **)malloc(size * sizeof(global_matrix_def));
  for (int i = 0; i < size; ++i) {
    matrix[i] = (global_matrix_def *)malloc(size * sizeof(global_matrix_def));
  }
  return matrix;
}

struct global_matrix_def** global_matrix_zeroed(int size) {
  struct global_matrix_def** matrix = global_matrix_alloc(size);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      matrix[i][j].real = 0;
      matrix[i][j].imag = 0;
    }
  }
  return matrix;
}

const char* global_get_gate_name(int gate) {
  return global_gate_names[gate];
}

const char* global_get_gate_type(int type) {
  return global_gate_types[type];
}
