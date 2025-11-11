import numpy as np
from ...qubit import qubit
from ..interface import CoreInterface


class BaseCore(CoreInterface):

    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        self.big_endian = big_endian
        self.qubits = qubits
        self.state = qubit(prep)
        for _ in range(qubits - 1):
            self.state = np.kron(self.state, qubit(prep))
        return

    def __operator_matrix__(self, gate_queue):
        matrix_to_calculate = gate_queue[0]
        for gate in gate_queue[1:]:
            matrix_to_calculate = np.kron(gate, matrix_to_calculate)
        self.state = np.dot(matrix_to_calculate, self.state)
        return

    def __multi_operator_matrix__(self, multi_gate_queue):
        bra_ket_zero_kron = multi_gate_queue[0]
        bra_ket_one_kron = multi_gate_queue[1]
        to_add_zero = bra_ket_zero_kron[0]
        to_add_one = bra_ket_one_kron[0]
        for i in range(1, len(bra_ket_zero_kron)):
            to_add_zero = np.kron(bra_ket_zero_kron[i], to_add_zero)
            to_add_one = np.kron(bra_ket_one_kron[i], to_add_one)
        self.state = np.dot(to_add_zero + to_add_one, self.state)
        return

    def __set_state__(self, state_to_store):
        self.state = state_to_store
        return
