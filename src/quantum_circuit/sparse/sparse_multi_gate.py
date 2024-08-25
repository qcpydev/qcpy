from scipy import sparse as sp
import numpy as np
from ...quantum_gate import identity
from ..interface import MultiGateInterface


class SparseMultiGate(MultiGateInterface):
    def __init__(self, qubits: int, big_endian: bool = False):
        self.qubits = qubits
        self.big_endian = big_endian

    def __create_braket_queue__(self, gate, control_qubit: int, target_qubit: int):
        gpu_identity = sp.csr_matrix(identity())
        braket_zero = sp.csr_matrix(np.array([[1 + 0j, 0 + 0j], [0 + 0j, 0 + 0j]], "F"))
        braket_one = sp.csr_matrix(np.array([[0 + 0j, 0 + 0j], [0 + 0j, 1 + 0j]], "F"))
        bra_ket_zero_kron = [gpu_identity] * self.qubits
        bra_ket_one_kron = [gpu_identity] * self.qubits
        bra_ket_zero_kron[control_qubit] = braket_zero
        bra_ket_one_kron[control_qubit] = braket_one
        bra_ket_one_kron[target_qubit] = gate
        if self.big_endian:
            bra_ket_zero_kron = bra_ket_zero_kron[::-1]
            bra_ket_one_kron = bra_ket_one_kron[::-1]
        return [bra_ket_zero_kron, bra_ket_one_kron]
