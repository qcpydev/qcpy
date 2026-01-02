import numpy as np
from qcpy import quantumcircuit


def inc(x, n):
    qc = quantumcircuit(qubits=x, prep="z")
    for i in range(x):
        for _ in range(n):
            qc.h(i)

    for i in range(x):
        for _ in range(n):
            qc.x(i)


def test_01a():
    inc(1, 20)


def test_01b():
    inc(2, 30)


def test_01c():
    inc(3, 40)


def test_01d():
    inc(4, 50)
