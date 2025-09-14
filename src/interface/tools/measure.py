from typing import Union
from numpy import arange, log2, ndarray, random
from ..quantum_circuit import QuantumCircuit
from .base import convert_state
from .probability import probability


def measure(quantumstate: Union[QuantumCircuit, ndarray]) -> str:
    """Outputs the measure of a quantum circuit state.
    ```
    from qcpy import quantumcircuit, measure
    measure(quantumcircuit(qubits = 2))
    ```
    Returns:
        NDArray: Amplitude array from given state.
    """
    state = convert_state(quantumstate)
    size = int(log2(state.size))
    probabilities = probability(state, round=size)
    final_state = random.choice(arange(len(state), dtype=int), 1, p=probabilities)
    final_state = str(bin(final_state[0]))[2:]
    final_state = final_state.zfill(size)
    return final_state
