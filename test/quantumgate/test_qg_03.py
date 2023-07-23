from qcpy.quantumgate import pauliy
import numpy as np


def test_qg_03():
    assert (
        pauliy() == np.array([
            [0 + 0j, 0 - 1j],
            [0 + 1j, 0 + 0j]
        ], 'F')
    ).all(), 'test_qg_03 Failed on PauliY'
