import numpy as np
from numpy.typing import NDArray


def qubit(initial: chr = "z") -> NDArray:
    """Create a vector that represents an array.

    Args:
        initial (chr, optional): The initial orientation of the array. Defaults to "z".

    Returns:
        NDArray: Vector array.
    """
    if initial == "z":
        return np.array([[1 + 0j], [0 + 0j]], "F")
    elif initial == "y":
        return np.array(
            [[0 + 0j + 1 + 0j], [(1 + 0j + 0 + 0j) * 0 + 1j]], "F"
        ) / np.sqrt(2)
    elif initial == "x":
        return np.array([[0 + 0j + 1 + 0j], [1 + 0j + 0 + 0j]], "F") / np.sqrt(2)
