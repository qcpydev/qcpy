import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z")
    for i in range(x):
        qc.h(i)
    for i in range(x):
        qc.t(i)
    return np.around(qc.state.flatten(), 3)


def test_03a():
    assert (
        inc(1) == np.array([0.707 + 0j, 0.5 + 0.5j], "F")
    ).all(), "test_03a Failed on hadamard and t"


def test_03b():
    assert (
        inc(2) == np.array([0.5 + 0j, 0.354 + 0.354j, 0.354 + 0.354j, 0 + 0.5j], "F")
    ).all(), "test_03b Failed on hadamard and t"


def test_03c():
    assert (
        inc(3)
        == np.array(
            [
                0.354 + 0j,
                0.25 + 0.25j,
                0.25 + 0.25j,
                0 + 0.354j,
                0.25 + 0.25j,
                0 + 0.354j,
                0 + 0.354j,
                -0.25 + 0.25j,
            ],
            "F",
        )
    ).all(), "test_03c Failed on hadamard and t"


def test_03d():
    assert (
        inc(4)
        == np.array(
            [
                0.25 + 0j,
                0.177 + 0.177j,
                0.177 + 0.177j,
                0 + 0.25j,
                0.177 + 0.177j,
                0 + 0.25j,
                0 + 0.25j,
                -0.177 + 0.177j,
                0.177 + 0.177j,
                0 + 0.25j,
                0 + 0.25j,
                -0.177 + 0.177j,
                0 + 0.25j,
                -0.177 + 0.177j,
                -0.177 + 0.177j,
                -0.25 + 0j,
            ],
            "F",
        )
    ).all(), "test_03d Failed on hadamard and t"
