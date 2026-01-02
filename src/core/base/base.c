#include <base.h>

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
    [TYPE_SINGLE] = "SINGLE",
    [TYPE_CONTROLLED] = "CONTROLLED",
    [TYPE_MULTI] = "MULTI",
    [TYPE_BLOCK] = "BLOCK",
    [TYPE_ALGORITHM] = "ALGORITHM",
};

const char *base_get_gate_str(int gate) { return base_gate_strs[gate]; }
const char *base_get_type_str(int type) { return base_type_strs[type]; }
