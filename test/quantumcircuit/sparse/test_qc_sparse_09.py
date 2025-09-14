import numpy as np

from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", sparse=True)
    qc.h(x - 1)
    qc.h(x - 2)
    qc.ccx(x - 1, x - 2, 0)
    return np.around(qc.state.flatten(), 3)


def test_09a():
    assert (
        inc(3)
        == np.array(
            [0.5 + 0j, 0 + 0j, 0.5 + 0j, 0 + 0j, 0.5 + 0j, 0 + 0j, 0 + 0j, 0.5 + 0j],
            "F",
        )
    ).all(), "test_09a Failed on hadamard -> hadamard -> toffoli"


def test_09b():
    assert (
        inc(4)
        == np.array(
            [
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_09b Failed on hadamard -> hadamard -> toffoli"


def test_09c():
    assert (
        inc(5)
        == np.array(
            [
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0.5 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_09c Failed on hadamard -> hadamard -> toffoli"
