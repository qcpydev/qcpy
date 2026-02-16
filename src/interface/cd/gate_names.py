from ...connect.port_entry import QuantumGates as QG

circuit_drawing_gate_names = {
    QG.IDENTITY: "I",
    QG.HADAMARD: "H",
    QG.PAULIX: "X",
    QG.PAULIY: "Y",
    QG.PAULIZ: "Z",
    QG.PHASE: "P",
    QG.S: "S",
    QG.SDG: "S†",
    QG.T: "T",
    QG.TDG: "T†",
    QG.RZ: "RZ",
    QG.RY: "RY",
    QG.RX: "RX",
    QG.SX: "SX",
    QG.SXDG: "SX†",
    QG.U: "U",
    QG.QFT: "QFT",
    QG.RCCX: "RCCX",
    QG.RC3X: "RC3X",
    QG.RXX: "RXX",
    QG.RZZ: "RZZ",
    QG.CR1: "R1",
    QG.CUSTOM: "C",
    QG.CUSTOMCONTROLLED: "CC",
    QG.MULTI: "M",
    QG.CUSTOMBLOCK: "CB",
    QG.CUSTOMALGORITHM: "CA",
    QG.SWAP: "SWAP",
}


def convert_gate_name(gate_name: int) -> str:
    gate_val = QG(gate_name)

    if gate_val in circuit_drawing_gate_names:
        return circuit_drawing_gate_names[gate_val]
    match gate_name:
        case QG.CX | QG.CCX:
            return circuit_drawing_gate_names[QG(QG.PAULIX)]
        case QG.CY:
            return circuit_drawing_gate_names[QG(QG.PAULIY)]
        case QG.CZ:
            return circuit_drawing_gate_names[QG(QG.PAULIZ)]
        case QG.CH:
            return circuit_drawing_gate_names[QG(QG.HADAMARD)]
        case QG.CRX:
            return circuit_drawing_gate_names[QG(QG.RX)]
        case QG.CRY:
            return circuit_drawing_gate_names[QG(QG.RY)]
        case QG.CRZ:
            return circuit_drawing_gate_names[QG(QG.RZ)]
        case QG.SWAP:
            return "SWAP"

    return ""


def convert_gate_type(gate_name: int) -> str:
    gate_val = QG(gate_name)
    match gate_val:
        case (
            QG.IDENTITY
            | QG.HADAMARD
            | QG.PAULIX
            | QG.PAULIY
            | QG.PAULIZ
            | QG.PAULIX
            | QG.PAULIY
            | QG.PHASE
            | QG.S
            | QG.SDG
            | QG.T
            | QG.TDG
            | QG.RZ
            | QG.RY
            | QG.RX
            | QG.SX
            | QG.SXDG
            | QG.U
            | QG.CUSTOM
        ):
            return "SINGLE"
        case (
            QG.CX
            | QG.CH
            | QG.CY
            | QG.CZ
            | QG.CRZ
            | QG.CRX
            | QG.CRY
            | QG.CSX
            | QG.CSXDG
            | QG.CR1
        ):
            return "CONTROLLED"
        case QG.CCX | QG.MULTI:
            return "MULTI"
        case QG.RCCX | QG.RC3X | QG.RXX | QG.RZZ | QG.CUSTOM:
            return "BLOCK"
        case QG.CUSTOMALGORITHM | QG.QFT:
            return "ALGORITHM"

    return "SINGLE"
