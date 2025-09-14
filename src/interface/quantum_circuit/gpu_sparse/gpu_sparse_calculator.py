import cupy as cp
from ..interface import CalculatorInterface
from .gpu_sparse_core import GpuSparseCore
from .gpu_sparse_single_gate import GpuSparseSingleGate
from .gpu_sparse_multi_gate import GpuSparseMultiGate


class GpuSparseCalculator(
    CalculatorInterface, GpuSparseCore, GpuSparseSingleGate, GpuSparseMultiGate
):

    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        GpuSparseCore.__init__(self, qubits, big_endian, prep)
        GpuSparseSingleGate.__init__(self, qubits, big_endian)
        GpuSparseMultiGate.__init__(self, qubits, big_endian)

    def pass_single_gate(self, qubits_to_apply, gate):
        self.__operator_matrix__(
            self.__create_gate_queue__(qubits_to_apply, cp.array(gate))
        )

    def pass_custom_gate_queue(self, gate_queue):
        gate_queue = [cp.array(i) for i in gate_queue]
        self.__operator_matrix__(gate_queue)

    def pass_multi_gate(self, control: int, target: int, gate):
        self.__multi_operator_matrix__(
            self.__create_braket_queue__(
                cp.array(gate),
                control,
                target,
            )
        )
