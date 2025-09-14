import numpy as np
from qcpy import gates


def test_qg_23():
    assert (
        gates.cz()
        == np.array(
            [
                [1 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 1 + 0j, 0 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 1 + 0j, 0 + 0j],
                [0 + 0j, 0 + 0j, 0 + 0j, -1 + 0j],
            ],
            "F",
        )
    ).all(), "test_qg_23 Failed on Cz"
