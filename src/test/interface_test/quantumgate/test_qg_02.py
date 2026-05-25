import numpy as np
from qcpy import gates


def test_qg_02():
    assert (
        gates.paulix() == np.array([[0 + 0j, 1 + 0j], [1 + 0j, 0 + 0j]], "F")
    ).all(), "test_qg_02 Failed on PauliX"
