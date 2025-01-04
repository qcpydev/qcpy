#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <arm_fp16.h>

typedef __fp16 float16;

typedef enum {
  GLOBAL_GATE_IDENTITY,
  GLOBAL_GATE_HADAMARD,
  GLOBAL_GATE_PAULIX,
  GLOBAL_GATE_PAULIY,
  GLOBAL_GATE_PAULIZ,
  GLOBAL_GATE_PHASE,
  GLOBAL_GATE_S,
  GLOBAL_GATE_SDG,
  GLOBAL_GATE_T,
  GLOBAL_GATE_TDG,
  GLOBAL_GATE_RZ,
  GLOBAL_GATE_RY,
  GLOBAL_GATE_RX,
  GLOBAL_GATE_SX,
  GLOBAL_GATE_SXDG,
  GLOBAL_GATE_U,
  GLOBAL_GATE_CX,
  GLOBAL_GATE_CH,
  GLOBAL_GATE_CY,
  GLOBAL_GATE_CZ,
  GLOBAL_GATE_CRX,
  GLOBAL_GATE_CRY,
  GLOBAL_GATE_CRZ,
  GLOBAL_GATE_CR1,
  GLOBAL_GATE_CCX,
  GLOBAL_GATE_QFT,
  GLOBAL_GATE_RCCX,
  GLOBAL_GATE_RC3X,
  GLOBAL_GATE_SWAP,
  GLOBAL_GATE_RXX,
  GLOBAL_GATE_RZZ,
  GLOBAL_GATE_CUSTOM,
  GLOBAL_GATE_CUSTOMCONTROLLED,
  GLOBAL_GATE_MULTI,
  GLOBAL_GATE_CUSTOMBLOCK,
  GLOBAL_GATE_CUSTOMALGORITHM
} global_gate_name;

typedef enum {
  GLOBAL_TYPE_SINGLE,
  GLOBAL_TYPE_CONTROLLED,
  GLOBAL_TYPE_MULTI,
  GLOBAL_TYPE_BLOCK,
  GLOBAL_TYPE_ALGORITHM,
} global_gate_type;

typedef enum {
  GLOBAL_MATRIX_DEL_INIT,
  GLOBAL_MATRIX_DEL_DEALLOC_ERR,
  GLOBAL_MATRIX_DEL_BAD_VALUE_ERR,
  GLOBAL_MATRIX_DEL_SUCCESS
} global_matrix_del_res;

typedef struct global_matrix_def {
  float16 real;
  float16 imag;
} global_matrix_def;

typedef struct global_state_def {
  struct global_matrix_def* state;
} global_state_def;

struct global_matrix_def** global_matrix_alloc(int size);
global_matrix_del_res global_matrix_delete(struct global_matrix_def **gate_matrix);
struct global_matrix_def** global_matrix_zeroed(int size);
