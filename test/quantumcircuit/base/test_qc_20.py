import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z")
    qc.h(x - 1)
    qc.rzz(x - 1, 0)
    qc.h(x - 1)
    return np.around(qc.state.flatten(), 3)


def test_20a():
    assert (
        inc(2) == np.array([0.5 + 0.5j, 0 + 0j, 0.5 - 0.5j, 0 + 0j], "F")
    ).all(), "test_20a Failed on hadamard -> rzz -> hadamard"


def test_20b():
    assert (
        inc(3)
        == np.array(
            [0.5 + 0.5j, 0 + 0j, 0 + 0j, 0 + 0j, 0.5 - 0.5j, 0 + 0j, 0 + 0j, 0 + 0j],
            "F",
        )
    ).all(), "test_20b Failed on hadamard -> rzz -> hadamard"


def test_20c():
    assert (
        inc(4)
        == np.array(
            [
                0.5 + 0.5j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 - 0.5j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_20c Failed on hadamard -> rzz -> hadamard"
