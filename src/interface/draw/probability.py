import re
from typing import Union
import matplotlib.pyplot as plt
from numpy import ndarray, log2
from ..quantum_circuit import QuantumCircuit
from ..errors import InvalidSavePathError
from ..tools import probability as prob
from .base import graph, light_mode, theme


def probability(
    quantumstate: Union[ndarray, QuantumCircuit],
    path: str = "probabilities.png",
    save: bool = False,
    show: bool = True,
    light: bool = False,
) -> None:
    """Creates a probability representation of a given quantum circuit in matplotlib.
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
    probabilities = prob(quantumstate)
    num_qubits = int(log2(probabilities.size))
    state_list = [format(i, "b").zfill(num_qubits) for i in range(2**num_qubits)]
    percents = [i * 100 for i in probabilities]
    plt.clf()
    plt.close()
    light_mode(light)
    ax = graph(theme.TEXT_COLOR, theme.BACKGROUND_COLOR, num_qubits)
    ax.bar(state_list, percents, color="#39c0ba")
    plt.xlabel("Computational basis states", color=theme.ACCENT_COLOR)
    plt.ylabel("Probability (%)", labelpad=5, color=theme.ACCENT_COLOR)
    plt.title("Probabilities", pad=10, color=theme.ACCENT_COLOR)
    plt.tight_layout()
    if save:
        plt.savefig(path)
    if show:
        plt.show()
    return
