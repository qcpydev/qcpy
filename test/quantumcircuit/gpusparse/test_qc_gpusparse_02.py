import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", gpu=True, sparse=True)
    for i in range(x):
        qc.h(i)
    return np.around(qc.state.flatten(), 3)


def test_02a():
    assert (
        inc(1) == np.array([0.707 + 0j, 0.707 + 0j], "F")
    ).all(), "test_02a Failed on hadamard"


def test_02b():
    assert (
        inc(2) == np.array([0.5 + 0j, 0.5 + 0j, 0.5 + 0j, 0.5 + 0j], "F")
    ).all(), "test_02b Failed on hadamard"


def test_02c():
    assert (
        inc(3)
        == np.array(
            [
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
            ],
            "F",
        )
    ).all(), "test_02c Failed on hadamard"


def test_02d():
    assert (
        inc(4)
        == np.array(
            [
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
                0.25 + 0j,
            ],
            "F",
        ).reshape(16, 1)
    ).all(), "test_02d Failed on hadamard"
