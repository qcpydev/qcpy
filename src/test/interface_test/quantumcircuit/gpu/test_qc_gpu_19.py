import numpy as np

from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", gpu=True)
    qc.h(0)
    qc.rzz(0, x - 1)
    qc.h(0)
    return np.around(qc.state.flatten(), 3)


def test_19a():
    assert (
        inc(2) == np.array([0.5 + 0.5j, 0.5 - 0.5j, 0 + 0j, 0 + 0j], "F")
    ).all(), "test_19a Failed on hadamard -> rzz -> hadamard"


def test_19b():
    assert (
        inc(3)
        == np.array(
            [0.5 + 0.5j, 0.5 - 0.5j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j],
            "F",
        )
    ).all(), "test_19b Failed on hadamard -> rzz -> hadamard"


def test_19c():
    assert (
        inc(4)
        == np.array(
            [
                0.5 + 0.5j,
                0.5 - 0.5j,
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
            ],
            "F",
        )
    ).all(), "test_19c Failed on hadamard -> rzz -> hadamard"
