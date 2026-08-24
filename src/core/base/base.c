#include <assert.h>
#include <base.h>
#include <stdlib.h>

#define BITPACK_MAX 0x3F
#define BITPACK_RANGE 6

const char *base_gate_strs[] = {
    [GATE_IDENTITY] = "IDENTITY",
    [GATE_HADAMARD] = "HADAMARD",
    [GATE_PAULIX] = "PAULIX",
    [GATE_PAULIY] = "PAULIY",
    [GATE_PAULIZ] = "PAULIZ",
    [GATE_PHASE] = "PHASE",
    [GATE_S] = "S",
    [GATE_SDG] = "SDG",
    [GATE_T] = "T",
    [GATE_TDG] = "TDG",
    [GATE_RZ] = "RZ",
    [GATE_RY] = "RY",
    [GATE_RX] = "RX",
    [GATE_SX] = "SX",
    [GATE_SXDG] = "SXDG",
    [GATE_U] = "U",
    [GATE_CX] = "CX",
    [GATE_CH] = "CH",
    [GATE_CY] = "CY",
    [GATE_CZ] = "CZ",
    [GATE_CRX] = "CRX",
    [GATE_CRY] = "CRY",
    [GATE_CRZ] = "CRZ",
    [GATE_CSX] = "CSX",
    [GATE_CR1] = "CR1",
    [GATE_CCX] = "CCX",
    [GATE_QFT] = "QFT",
    [GATE_RCCX] = "RCCX",
    [GATE_RC3X] = "RC3X",
    [GATE_SWAP] = "SWAP",
    [GATE_RXX] = "RXX",
    [GATE_RZZ] = "RZZ",
    [GATE_CUSTOM] = "CUSTOM",
    [GATE_CUSTOMCONTROLLED] = "CUSTOMCONTROLLED",
    [GATE_MULTI] = "MULTI",
    [GATE_CUSTOMBLOCK] = "CUSTOMBLOCK",
    [GATE_CUSTOMALGORITHM] = "CUSTOMALGORITHM",
};

const char *base_type_strs[] = {
    [TYPE_SINGLE] = "SINGLE",       [TYPE_CONTROLLED] = "CONTROLLED",
    [TYPE_MULTI] = "MULTI",         [TYPE_BLOCK] = "BLOCK",
    [TYPE_ALGORITHM] = "ALGORITHM",
};

const char *base_get_gate_str(base_gate_e gate) { return base_gate_strs[gate]; }
const char *base_get_type_str(base_type_e type) { return base_type_strs[type]; }

bitmask_t base_create_qubit_bitmask(qubit_t *to_bitmask, qubit_t qubits) {
  bitmask_t bitmask = 0;

  for (qubit_t i = 0; i < qubits; ++i) {
    bitmask |= (bitmask_t)1 << to_bitmask[i];
  }

  return bitmask;
}

bitpack_t base_create_qubit_bitpack(qubit_t *to_bitpack, qubit_t count) {
  return 0;
}

qubit_t *base_decompress_qubit_bitpack(qubit_t qubits, bitpack_t bitpacked) {
  qubit_t *unpacked = NULL;
  unpacked = (qubit_t *)malloc(2 * sizeof(qubits));

  for (qubit_t i = 0; i < qubits; ++i) {
    unpacked[i] = (bitpacked >> (i * BITPACK_RANGE)) & BITPACK_MAX;
  }

  return unpacked;
}

qubit_t *base_unpack_qubit_bitmask(bitmask_t bitmasked, qubit_t count) {
  qubit_t *qubits = (qubit_t *)malloc(sizeof(qubit_t) * count);

  qubit_t qubit = 0;
  for (bitmask_t i = 0; i < count; i <<= 1) {
    if (i & bitmasked) {
      qubits[i] = qubit;
    }
    ++qubit;
  }
  return qubits;
}
