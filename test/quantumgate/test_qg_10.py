import numpy as np

from qcpy import gates


def test_qg_10():
    assert (
        gates.s() == np.array([[1 + 0j, 0 + 0j], [0 + 0j, 0 + 1j]], "F")
    ).all(), "test_qg_10 Failed on S"
