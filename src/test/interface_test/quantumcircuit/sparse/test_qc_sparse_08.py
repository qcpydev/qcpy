import numpy as np

from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", sparse=True)
    qc.h(0)
    qc.h(1)
    qc.ccx(0, 1, x - 1)
    return np.around(qc.state.flatten(), 3)


def test_08a():
    assert (
        inc(3)
        == np.array(
            [0.5 + 0j, 0.5 + 0j, 0.5 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0 + 0j, 0.5 + 0j],
            "F",
        )
    ).all(), "test_08a Failed on hadamard -> hadamard -> toffoli"


def test_08b():
    assert (
        inc(4)
        == np.array(
            [
                0.5 + 0j,
                0.5 + 0j,
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
            ],
            "F",
        )
    ).all(), "test_08b Failed on hadamard -> hadamard -> toffoli"


def test_08c():
    assert (
        inc(5)
        == np.array(
            [
                0.5 + 0j,
                0.5 + 0j,
                0.5 + 0j,
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
                0.5 + 0j,
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
    ).all(), "test_08c Failed on hadamard -> hadamard -> toffoli"
