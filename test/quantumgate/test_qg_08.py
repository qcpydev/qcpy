import numpy as np

from qcpy import gates


def test_qg_08():
    assert (
        gates.toffoli()
        == np.array(
            [
                [1 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 1 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 1 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 1 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 1 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 1 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 1 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 1 + 0j, 0 + 0j],
            ],
            "F",
        )
    ).all(), "test_qg_08 Failed on Toffoli"
