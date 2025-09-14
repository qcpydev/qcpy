from numpy import angle, around, mod, pi, log2, ndarray
from numpy.typing import NDArray
from .base import convert_state
from typing import Union
from ..quantum_circuit import QuantumCircuit
from ..errors import RoundBelowZeroError, OutOfRangeError


def phase_angle(
    quantumstate: Union[ndarray, QuantumCircuit],
    show_bit: Union[str, int] = -1,
    round: int = 3,
    radian: bool = True,
) -> NDArray:
    """Outputs the phase angle of a quantum circuit state.

    ```
    from qcpy import quantumcircuit, phaseangle
    phaseangle(quantumcircuit(qubits = 2)
    ```
    Args:
        state: (ndarray/quantumcircuit, required): State to convert to a phase angle array. No default.
        show_bit: (str/int, optional): Can specify showing a single value from the state. Defaults to -1.
        round: (int, optional): Rounds the phase angle array by a given value. Defaults to 3.
        radian: (bool, true): Can represent the phase angle array in radians or not. Defaults to true.
    Returns:
        NDArray: Phase angle array from given state.
    """
    state = convert_state(quantumstate)
    size = int(log2(quantumstate.size))

    if round < 0:
        raise RoundBelowZeroError(f"Cannot round to {round} needs to be 0 or greater")

    if isinstance(show_bit, int) and show_bit < 0:
        phaseangle = mod(angle(state), 2 * pi) * (180 / pi)

    elif isinstance(show_bit, str):
        phaseangle = int(show_bit, 2)
        if 2**size <= phase_angle:
            raise OutOfRangeError(
                f"Cannot show bit of value {show_bit} as it is out of range"
            )
        phaseangle = mod(angle(state[phaseangle]), 2 * pi) * (180 / pi)

    elif isinstance(show_bit, int):
        phaseangle = show_bit
        if 2**size <= phase_angle:
            raise OutOfRangeError(
                f"Cannot show bit of value {show_bit} as it is out of range"
            )
        phaseangle = mod(angle(state[phaseangle]), 2 * pi) * (180 / pi)

    if radian:
        phaseangle *= pi / 180

    return around(phaseangle, decimals=round)
