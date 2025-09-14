import numpy as np
from qcpy import gates


def test_qg_22():
    assert (
        gates.cr()
        == np.array(
            [
                [1 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 1 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 1 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, np.exp((np.pi / 2) * 0 + 1j)],
            ],
            "F",
        )
    ).all(), "test_qg_22 Failed on Cr"
