from numpy import abs, around, multiply, square, log2, ndarray
from numpy.typing import NDArray
from typing import Union
from .base import convert_state
from ..quantum_circuit import QuantumCircuit
from ..errors import InvalidProbability, RoundBelowZeroError, OutOfRangeError


def probability(
    quantumstate: Union[ndarray, QuantumCircuit],
    show_percent: bool = False,
    show_bit: int = -1,
    round: int = 3,
) -> NDArray:
    """Outputs the probability of a quantum circuit state.

    ```
    from qcpy import quantumcircuit, probability
    probablity(quantumcircuit(qubits = 2)
    ```
    Args:
        state: (ndarray/quantumcircuit, required): State to convert to a probability array. No default.
        show_bit: (str/int, optional): Can specify showing a single value from the state. Defaults to -1.
        round: (int, optional): Rounds the phase angle array by a given value. Defaults to 3.
        radian: (bool, true): Can represent the phase angle array in radians or not. Defaults to true.
    Returns:
        NDArray: Probability array from given state.
    """
    circuit_size = int(log2(quantumstate.size))
    quantum_state = convert_state(quantumstate)
    if round < 0:
        raise RoundBelowZeroError(f"Cannot round to {round} needs to be 0 or greater")
    if isinstance(show_bit, int) and show_bit < 0:
        probability = abs(square(quantum_state))
    elif isinstance(show_bit, str) or isinstance(show_bit, int):
        if 2**circuit_size <= show_bit:
            raise OutOfRangeError(
                f"Cannot show bit of value {show_bit} as it is out of range"
            )
        probability = abs(square(quantum_state[show_bit]))
    if show_percent:
        probability = multiply(probability, 100)
    output = around(probability, decimals=round)
    return output
