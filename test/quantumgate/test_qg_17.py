import numpy as np

from qcpy import gates


def test_qg_17():
    assert (
        gates.sx() == np.array([[1 + 1j, 1 - 1j], [1 - 1j, 1 + 1j]], "F") * (1 / 2)
    ).all(), "test_qg_17 Failed on Sx"
