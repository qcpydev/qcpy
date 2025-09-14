import numpy as np

from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", gpu=True, sparse=True)
    qc.h(0)
    qc.h(1)
    qc.h(2)
    qc.rc3x(0, 1, 2, x - 1)
    return np.around(qc.state.flatten(), 3)


def test_17a():
    assert (
        inc(4)
        == np.array(
            [
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0 + 0.354j,
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                -0.354 + 0j,
            ],
            "F",
        )
    ).all(), "test_17a Failed on hadamard (x3) -> rc3x"


def test_17b():
    assert (
        inc(5)
        == np.array(
            [
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0 + 0.354j,
                0.354 + 0j,
                0.354 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                -0.354 + 0j,
                0 + 0j,
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
    ).all(), "test_17b Failed on hadamard (x3) -> rc3x"
