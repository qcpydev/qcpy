import numpy as np
from qcpy import gates


def test_qg_20():
    assert (
        gates.rxx()
        == np.array(
            [
                [
                    np.cos((np.pi / 2) / 2),
                    0 + 0j,
                    0 + 0j,
                    0 - 1j * np.sin((np.pi / 2) / 2),
                ],
                [
                    0 + 0j,
                    np.cos((np.pi / 2) / 2),
                    0 - 1j * np.sin((np.pi / 2) / 2),
                    0 + 0j,
                ],
                [
                    0 + 0j,
                    0 - 1j * np.sin((np.pi / 2) / 2),
                    np.cos((np.pi / 2) / 2),
                    0 + 0j,
                ],
                [
                    0 - 1j * np.sin((np.pi / 2) / 2),
                    0 + 0j,
                    0 + 0j,
                    np.cos((np.pi / 2) / 2),
                ],
            ],
            "F",
        )
    ).all(), "test_qg_20 Failed on Rxx"
