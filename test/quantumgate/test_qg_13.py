import numpy as np
from qcpy import gates


def test_qg_13():
    assert (
        gates.tdg()
        == np.array([[1 + 0j, 0 + 0j], [0 + 0j, np.exp((0 - 1j * np.pi) / 4)]], "F")
    ).all(), "test_qg_13 Failed on Tdg"
