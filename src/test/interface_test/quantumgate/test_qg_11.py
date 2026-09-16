import numpy as np

from qcpy import gates


def test_qg_11():
    assert (
        gates.sdg() == np.array([[1 + 0j, 0 + 0j], [0 + 0j, 0 - 1j]], "F")
    ).all(), "test_qg_11 Failed on Sdg"
