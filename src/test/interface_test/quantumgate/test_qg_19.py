import numpy as np

from qcpy import gates


def test_qg_19():
    assert (
        gates.u()
        == np.array(
            [
                [
                    np.cos((np.pi / 2) / 2),
                    -1 * np.exp(0 + 1j * (np.pi / 2)) * np.sin((np.pi / 2) / 2),
                ],
                [
                    np.exp(0 + 1j * (np.pi / 2)) * np.sin((np.pi / 2) / 2),
                    np.exp(0 + 1j * ((np.pi / 2) + (np.pi / 2)))
                    * np.cos((np.pi / 2) / 2),
                ],
            ],
            "F",
        )
    ).all(), "test_qg_19 Failed on U"
