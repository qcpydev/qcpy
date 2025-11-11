import cupy as cp
from cupyx.scipy.sparse import csr_matrix, csc_matrix
from cupyx.scipy.sparse import kron
from ..interface import CoreInterface
from ...qubit import qubit


class GpuSparseCore(CoreInterface):

    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        self.big_endian = big_endian
        self.qubits = qubits
        gpusparse_qubit = csc_matrix(cp.array(qubit(prep)))
        self.state = gpusparse_qubit
        for _ in range(qubits - 1):
            self.state = kron(self.state, gpusparse_qubit)

    def __operator_matrix__(self, gate_queue):
        matrix_to_calculate = gate_queue[0]
        for gate in gate_queue[1:]:
            matrix_to_calculate = kron(gate, matrix_to_calculate)
        self.state = csc_matrix(matrix_to_calculate @ self.state.toarray())
        return

    def __multi_operator_matrix__(self, multi_gate_queue):
        bra_ket_zero_kron = multi_gate_queue[0]
        bra_ket_one_kron = multi_gate_queue[1]
        to_add_zero = csr_matrix(bra_ket_zero_kron[0])
        to_add_one = csr_matrix(bra_ket_one_kron[0])
        for i in range(1, len(bra_ket_zero_kron)):
            to_add_zero = kron(csr_matrix(bra_ket_zero_kron[i]), to_add_zero)
            to_add_one = kron(csr_matrix(bra_ket_one_kron[i]), to_add_one)
        to_add_zero += to_add_one
        self.state = csc_matrix(to_add_zero @ self.state.toarray())
