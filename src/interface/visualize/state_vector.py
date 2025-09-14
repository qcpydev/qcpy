import matplotlib.pyplot as plt
from numpy import log2, ndarray
from numpy import log2, ndarray, amax, pi
from matplotlib.colors import rgb2hex
from typing import Union
import re
from ..quantum_circuit import QuantumCircuit
from ..errors import InvalidSavePathError
from .base.graph import graph
from ..tools import amplitude, phaseangle
from .base import color_bar, theme, light_mode


def state_vector(
    quantumstate: Union[ndarray, QuantumCircuit],
    path: str = "statevector.png",
    save: bool = False,
    show: bool = True,
    light: bool = False,
) -> None:
    """Outputs a state vector representation from a given quantum circuit in matplotlib.
    Args:
        quantum_state (ndarray/QuantumCircuit): State vector array or qcpy quantum circuit.
        path (str): The path in which the image file will be saved when save is set true.
        save (bool): Will save an image in the working directory when this boolean is true.
        show (bool): Boolean to turn on/off the qsphere being opened in matplotlib.
        light (bool): Will change the default dark theme mode to a light theme mode.
    Returns:
        None
    """
    if save and re.search(r"[<>:/\\|?*]", path) or len(path) > 255:
        raise InvalidSavePathError("Invalid file name")
    amplitudes = amplitude(quantumstate)
    phase_angles = phaseangle(quantumstate)
    num_qubits = int(log2(amplitudes.size))
    state_list = [format(i, "b").zfill(num_qubits) for i in range(2**num_qubits)]
    light_mode(light)
    ax = graph(theme.TEXT_COLOR, theme.BACKGROUND_COLOR, num_qubits)
    ax.set_ylim(0, amax(amplitudes))
    norm = plt.Normalize(0, pi * 2)
    colors = plt.get_cmap("hsv")
    color_bar(plt, theme.TEXT_COLOR, theme.ACCENT_COLOR, colors, norm)
    hex_arr = [rgb2hex(i) for i in colors(norm(phase_angles))]
    ax.bar(state_list, amplitudes, color=hex_arr)
    plt.xlabel("Computational basis states", color=theme.TEXT_COLOR)
    plt.ylabel("Amplitutde", labelpad=5, color=theme.TEXT_COLOR)
    plt.title("State Vector", pad=10, color=theme.TEXT_COLOR)
    plt.tight_layout()
    if save:
        plt.savefig(path)
    if show:
        plt.show()
    return
