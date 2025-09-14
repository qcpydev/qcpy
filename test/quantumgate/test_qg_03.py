import numpy as np

from qcpy import gates


def test_qg_03():
    assert (
        gates.pauliy() == np.array([[0 + 0j, 0 - 1j], [0 + 1j, 0 + 0j]], "F")
    ).all(), "test_qg_03 Failed on PauliY"
