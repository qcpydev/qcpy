import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", gpu=True, sparse=True)
    qc.h(x - 2)
    qc.h(x - 1)
    qc.rccx(x - 1, x - 2, 0)
    return np.around(qc.state.flatten(), 3)


def test_15a():
    assert (
        inc(3)
        == np.array(
            [0.5 + 0j, 0 + 0j, 0.5 + 0j, 0 + 0j, 0.5 + 0j, 0 + 0j, 0 + 0j, 0 + 0.5j],
            "F",
        )
    ).all(), "test_15a Failed hadamard -> hadamard -> rccx"


def test_15b():
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
                0 + 0.5j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_15b Failed hadamard -> hadamard -> rccx"


def test_15c():
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
                0 + 0.5j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
                0 + 0j,
            ],
            "F",
        )
    ).all(), "test_15c Failed hadamard -> hadamard -> rccx"
