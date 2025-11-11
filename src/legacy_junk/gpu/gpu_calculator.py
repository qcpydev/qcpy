import cupy as cp
from ..interface import CalculatorInterface
from .gpu_core import GpuCore
from .gpu_single_gate import GpuSingleGate
from .gpu_multi_gate import GpuMultiGate


class GpuCalculator(CalculatorInterface, GpuCore, GpuSingleGate, GpuMultiGate):

    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        GpuCore.__init__(self, qubits, big_endian, prep)
        GpuSingleGate.__init__(self, qubits, big_endian)
        GpuMultiGate.__init__(self, qubits, big_endian)

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
