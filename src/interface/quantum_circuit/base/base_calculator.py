import numpy as np
from ..interface import CalculatorInterface
from .base_core import BaseCore
from .base_multi_gate import BaseMultiGate
from .base_single_gate import BaseSingleGate


class BaseCalculator(CalculatorInterface, BaseCore, BaseSingleGate, BaseMultiGate):

    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        BaseCore.__init__(self, qubits, big_endian, prep)
        BaseSingleGate.__init__(self, qubits, big_endian)
        BaseMultiGate.__init__(self, qubits, big_endian)

    def pass_single_gate(self, qubits_to_apply, gate: np.array):
        self.__operator_matrix__(self.__create_gate_queue__(qubits_to_apply, gate))

    def pass_custom_gate_queue(self, gate_queue):
        self.__operator_matrix__(gate_queue)

    def pass_multi_gate(self, control: int, target: int, gate: np.array):
        self.__multi_operator_matrix__(
            self.__create_braket_queue__(control, target, gate)
        )
