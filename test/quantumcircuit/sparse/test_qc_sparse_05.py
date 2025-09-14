import numpy as np

from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", sparse=True)
    qc.h(x - 1)
    qc.cx(x - 1, 0)
    return np.around(qc.state.flatten(), 3)


def test_05a():
    assert (
        inc(2) == np.array([0.707 + 0j, 0 + 0j, 0 + 0j, 0.707 + 0j], "F")
    ).all(), "test_05a Failed on hadamard and cnot"


def test_05b():
    assert (
        inc(3)
        == np.array(
            [0.707 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0.707 + 0j, 0 + 0j, 0 + 0j],
            "F",
        )
    ).all(), "test_05b Failed on hadamard and cnot"


def test_05c():
    assert (
        inc(4)
        == np.array(
            [
                0.707 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.707 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_05c Failed on hadamard and cnot"
