from numpy import arcsin, around, power, sqrt, log2, ndarray
from numpy.typing import NDArray
from .base import convert_state
from typing import Union
from ..quantum_circuit import QuantumCircuit
from ..errors import RoundBelowZeroError, OutOfRangeError


def amplitude(
    quantumstate: Union[ndarray, QuantumCircuit],
    show_bit: Union[int, str] = -1,
    rounds: int = 3,
    radian: bool = False,
) -> NDArray:
    """Outputs the amplitude of a quantum circuit state.
    ```
    from qcpy import quantumcircuit, amplitude
    amplitude(quantumcircuit(qubits = 2)
    ```
    Args:
        state: (ndarray/quantumcircuit, required): State to convert to a amplitude array. No default.
        show_bit: (str/int, optional): Can specify showing a single value from the state. Defaults to -1.
        round: (int, optional): Rounds the amplitude array by a given value. Defaults to 3.
        radian: (bool, true): Can represent the amplitude array in radians or not. Defaults to true.
    Returns:
        NDArray: Amplitude array from given state.
    """
    state = convert_state(quantumstate)
    size = int(log2(state.size))

    if rounds < 0:
        raise RoundBelowZeroError(f"Cannot round to {round} needs to be 0 or greater")

    if isinstance(show_bit, int) and show_bit < 0:
        amplitude = sqrt(power(state.real, 2) + power(state.imag, 2))

    elif isinstace(show_bit, str):
        amplitude = int(show_bit, 2)
        if 2**size <= amplitude:
            raise OutOfRangeError(
                f"Cannot show bit of value {show_bit} as it is out of range"
            )
        amplitude = sqrt(
            power(state[amplitude].real, 2) + power(state[amplitude].imag, 2)
        )

    elif isinstance(show_bit, int):
        amplitude = show_bit
        if 2**size <= amplitude:
            raise OutOfRangeError(
                f"Cannot show bit of value {show_bit} as it is out of range"
            )
        amplitude = sqrt(
            power(quantumstate[amplitude].real, 2)
            + power(quantumstate[amplitude].imag, 2)
        )

    if radian:
        amplitude = arcsin(amplitude) * 2

    return around(amplitude, decimals=rounds)
