import re
import matplotlib.pyplot as plt
import numpy as np
from typing import Union
from numpy._core.multiarray import ndarray
from ..quantum_circuit import QuantumCircuit
from ..errors import BlochSphereOutOfRangeError, InvalidSavePathError
from ..tools import amplitude, probability
from .base import light_mode, sphere, theme


def bloch(
    quantumstate: Union[ndarray, QuantumCircuit],
    path: str = "BlochSphere.png",
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
    amplitudes = amplitude(quantumstate)
    phase_angles = probability(quantumstate, False)
    if amplitudes.size > 2:
        BlochSphereOutOfRangeError(
            "Bloch sphere only accepts a single qubit quantum circuit"
        )
    light_mode(light)
    ax = sphere(theme.BACKGROUND_COLOR)
    ax.quiver(1, 0, 0, 0.75, 0, 0, color="lightgray")
    ax.text(2, 0, 0, "+x", color="gray")
    ax.quiver(0, 1, 0, 0, 0.75, 0, color="lightgray")
    ax.text(0, 2, 0, "+y", color="gray")
    ax.quiver(0, 0, 1, 0, 0, 0.75, color="lightgray")
    ax.text(0, 0, 2, "+z", color="gray")
    ax.text(0.1, 0, 1.5, "|0>", color="gray")
    ax.quiver(0, 0, -1, 0, 0, -0.75, color="lightgray")
    ax.text(0, 0, -2, "-z", color="gray")
    ax.text(0.1, 0, -1.5, "|1>", color="gray")
    theta = np.arcsin(amplitudes[1]) * 2
    phi = phase_angles[1]
    x = 1 * np.sin(theta) * np.cos(phi)
    y = 1 * np.sin(theta) * np.sin(phi)
    z = 1 * np.cos(theta)
    xs, ys, zs = [0, x], [0, y], [0, z]
    ax.plot3D(xs, ys, zs, color=theme.ACCENT_COLOR, markevery=100)
    ax.scatter(xs[1], ys[1], zs[1], s=5, color=theme.ACCENT_COLOR)
    ax.text(xs[1] * 1.15, ys[1] * 1.15, zs[1] * 1.15, "|ψ⟩", color=theme.ACCENT_COLOR)
    plt.tight_layout()
    plt.axis("off")
    if save:
        plt.savefig(path)
    if show:
        plt.show()
    return
