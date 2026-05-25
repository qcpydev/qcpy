import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", gpu=True, sparse=True)
    qc.h(x - 1)
    qc.cx(x - 1, 0)
    qc.h(x - 1)
    return np.around(qc.state.flatten(), 3)


def test_07a():
    assert (
        inc(2) == np.array([0.5 + 0j, 0.5 + 0j, 0.5 + 0j, -0.5 + 0j], "F")
    ).all(), "test_07a Failed on hadamard -> cnot -> hadamard"


def test_07b():
    assert (
        inc(3)
        == np.array(
            [0.5 + 0j, 0.5 + 0j, 0 + 0j, 0 + 0j, 0.5 + 0j, -0.5 + 0j, 0 + 0j, 0 + 0j],
            "F",
        )
    ).all(), "test_07b Failed on hadamard -> cnot -> hadamard"


def test_07c():
    assert (
        inc(4)
        == np.array(
            [
                0.5 + 0j,
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 + 0j,
                -0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_07c Failed on hadamard -> cnot -> hadamard"
