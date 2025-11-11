from scipy import sparse as sp
from ..interface import CalculatorInterface
from .sparse_core import SparseCore
from .sparse_single_gate import SparseSingleGate
from .sparse_multi_gate import SparseMultiGate


class SparseCalculator(
    CalculatorInterface, SparseCore, SparseSingleGate, SparseMultiGate
):

    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        SparseCore.__init__(self, qubits, big_endian, prep)
        SparseSingleGate.__init__(self, qubits, big_endian)
        SparseMultiGate.__init__(self, qubits, big_endian)

    def pass_single_gate(self, qubits_to_apply, gate):
        self.__operator_matrix__(
            self.__create_gate_queue__(qubits_to_apply, sp.csr_matrix(gate))
        )

    def pass_multi_gate(self, control: int, target: int, gate):
        self.__multi_operator_matrix__(
            self.__create_braket_queue__(sp.csr_matrix(gate), control, target)
        )

    def pass_custom_gate_queue(self, gate_queue):
        gate_queue = [sp.csr_matrix(i) for i in gate_queue]
        self.__operator_matrix__(gate_queue)
