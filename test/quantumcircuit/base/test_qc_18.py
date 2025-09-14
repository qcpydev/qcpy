import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z")
    qc.h(x - 1)
    qc.h(x - 2)
    qc.h(x - 3)
    qc.rc3x(x - 1, x - 2, x - 3, 0)
    return np.around(qc.state.flatten(), 3)


def test_18a():
    assert (
        inc(4)
        == np.array(
            [
                0.354 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0.354j,
                0 + 0j,
                0 + 0j,
                -0.354 + 0j,
            ],
            "F",
        )
    ).all(), "test_18a Failed hadamard (x3) -> rc3x"


def test_18b():
    assert (
        inc(5)
        == np.array(
            [
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.354 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0.354j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                -0.354 + 0j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_18b Failed hadamard (x3) -> rc3x"
