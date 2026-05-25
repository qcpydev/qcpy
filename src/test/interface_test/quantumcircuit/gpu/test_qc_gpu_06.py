import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", gpu=True)
    qc.h(0)
    qc.cx(0, x - 1)
    qc.h(0)
    return np.around(qc.state.flatten(), 3)


def test_06a():
    assert (
        inc(2) == np.array([0.5 + 0j, 0.5 + 0j, 0.5 + 0j, -0.5 + 0j], "F")
    ).all(), "test_06a Failed on hadamard -> cnot -> hadamard"


def test_06b():
    assert (
        inc(3)
        == np.array(
            [0.5 + 0j, 0.5 + 0j, 0 + 0j, 0 + 0j, 0.5 + 0j, -0.5 + 0j, 0 + 0j, 0 + 0j],
            "F",
        )
    ).all(), "test_06b Failed on hadamard -> cnot -> hadamard"


def test_06c():
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
    ).all(), "test_06c Failed on hadamard -> cnot -> hadamard"
