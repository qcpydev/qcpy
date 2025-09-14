import cupy as cp
from ...quantum_gate import identity
from ..interface import SingleGateInterface


class GpuSparseSingleGate(SingleGateInterface):

    def __init__(self, qubits: int, big_endian: bool = False):
        self.qubits = qubits
        self.big_endian = big_endian
        return

    def __create_gate_queue__(self, qubits_to_apply, gate: cp.array):
        gpu_identity = cp.array(identity())
        gate_queue = cp.array([gpu_identity] * self.qubits)
        if isinstance(qubits_to_apply, int):
            gate_queue[qubits_to_apply] = gate
        elif hasattr(qubits_to_apply, "__len__"):
            for i in qubits_to_apply:
                gate_queue[i] = gate
        if self.big_endian:
            gate_queue = gate_queue[::-1]
        return gate_queue
