from scipy import sparse as sp
from ..interface import CoreInterface
from ...qubit import qubit


class SparseCore(CoreInterface):
    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        self.big_endian = big_endian
        self.qubits = qubits
        sparse_qubit = sp.csc_matrix(qubit(prep))
        self.state = sp.csc_matrix(sparse_qubit)
        for _ in range(qubits - 1):
            self.state = sp.csc_matrix(sp.kron(self.state, sparse_qubit))

    def __operator_matrix__(self, gate_queue):
        matrix_to_calculate = gate_queue[0]
        for gate in gate_queue[1:]:
            matrix_to_calculate = sp.kron(gate, matrix_to_calculate)
        self.state = sp.csc_matrix(matrix_to_calculate.dot(self.state))
        return

    def __multi_operator_matrix__(self, multi_gate_queue):
        bra_ket_zero_kron = multi_gate_queue[0]
        bra_ket_one_kron = multi_gate_queue[1]
        to_add_zero = bra_ket_zero_kron[0]
        to_add_one = bra_ket_one_kron[0]
        for i in range(1, len(bra_ket_zero_kron)):
            to_add_zero = sp.kron(bra_ket_zero_kron[i], to_add_zero)
            to_add_one = sp.kron(bra_ket_one_kron[i], to_add_one)
        to_add_zero += to_add_one
        self.state = sp.csc_matrix(to_add_zero.dot(self.state))
        return
