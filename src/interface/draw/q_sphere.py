import matplotlib.pyplot as plt
from numpy import pi, log2, ndarray, cos, sin, linspace, ndarray
import math
import re
from typing import Union
from ..qc import QuantumCircuit
from ..error import InvalidSavePathError
from .base import (
    sphere,
    color_bar,
    theme,
    light_mode,
)
from ..tools import probability, phaseangle


def q_sphere(
    quantumstate: Union[ndarray, QuantumCircuit],
    path: str = "qsphere.png",
    save: bool = False,
    show: bool = True,
    light: bool = False,
) -> None:
    """Creates a qsphere visualization that can be interacted with.
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
    colors = plt.get_cmap("hsv")
    norm = plt.Normalize(0, pi * 2)
    ax = sphere(theme.BACKGROUND_COLOR)
    light_mode(light)
    color_bar(plt, theme.TEXT_COLOR, theme.ACCENT_COLOR, colors, norm)
    prob_values = probability(quantumstate)
    phase_values = phaseangle(quantumstate)
    num_qubits = int(log2(len(prob_values)))
    bucket_array = [0.0] * (num_qubits + 1)
    left = 0
    right = num_qubits
    while left < right:
        binomial_coeff = math.comb(num_qubits, left)
        inc_segment = 2 * pi / binomial_coeff
        bucket_array[left] += binomial_coeff / inc_segment
        bucket_array[right] += binomial_coeff / inc_segment * -1
        left += 1
        right -= 1
    phi_values = linspace(0, pi, num_qubits + 1)
    for i in range(len(prob_values)):
        if prob_values[i] > 0:
            binary_string = format(i, "b").zfill(num_qubits)
            bucket_index = binary_string.count("1")
            binomial_coeff = math.comb(num_qubits, bucket_index)
            inc_segment = 2 * pi / binomial_coeff
            lat_segments = inc_segment + bucket_array[bucket_index]
            bucket_array[bucket_index] += inc_segment
            x = [0, sin(phi_values[bucket_index]) * cos(lat_segments)]
            y = [0, sin(phi_values[bucket_index]) * sin(lat_segments)]
            z = [0, cos(phi_values[bucket_index])]
            ax.plot3D(x, y, z, color=colors(norm(phase_values[i])))
            ax.scatter(x[1], y[1], z[1], color=colors(norm(phase_values[i])))
            ax.text(
                x[1] * 1.15,
                y[1] * 1.15,
                z[1] * 1.15,
                f"|{binary_string}>",
                color=theme.TEXT_COLOR,
            )
    plt.tight_layout()
    plt.axis("off")
    if save:
        plt.savefig(path)
    if show:
        plt.show()
    return
