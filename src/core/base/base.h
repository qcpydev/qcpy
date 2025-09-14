#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef GLOBAL_H
extern bool qcpy_debug_mode;

/*
 * A space to set any common or useful functions/types/etc.
 */

typedef enum {
  GATE_IDENTITY,
  GATE_HADAMARD,
  GATE_PAULIX,
  GATE_PAULIY,
  GATE_PAULIZ,
  GATE_PHASE,
  GATE_S,
  GATE_SDG,
  GATE_T,
  GATE_TDG,
  GATE_RZ,
  GATE_RY,
  GATE_RX,
  GATE_SX,
  GATE_SXDG,
  GATE_U,
  GATE_CX,
  GATE_CH,
  GATE_CY,
  GATE_CZ,
  GATE_CRX,
  GATE_CRY,
  GATE_CRZ,
  GATE_CSX,
  GATE_CSXDG,
  GATE_CR1,
  GATE_CCX,
  GATE_QFT,
  GATE_RCCX,
  GATE_RC3X,
  GATE_SWAP,
  GATE_RXX,
  GATE_RZZ,
  GATE_CUSTOM,
  GATE_CUSTOMCONTROLLED,
  GATE_MULTI,
  GATE_CUSTOMBLOCK,
  GATE_CUSTOMALGORITHM
} base_gate_e;

typedef enum {
  TYPE_SINGLE,
  TYPE_CONTROLLED,
  TYPE_MULTI,
  TYPE_BLOCK,
  TYPE_ALGORITHM,
} base_type_e;

const char *base_get_gate_str(int gate);
const char *base_get_type_str(int type);
#endif // GLOBAL_H
