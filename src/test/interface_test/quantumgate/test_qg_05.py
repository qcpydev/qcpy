import numpy as np

from qcpy import gates


def test_qg_05():
    assert (
        gates.hadamard()
        == np.array([[1 + 0j, 1 + 0j], [1 + 0j, -1 + 0j]], "F") * (1 / np.sqrt(2))
    ).all(), "test_qg_05 Failed on Hadamard"
