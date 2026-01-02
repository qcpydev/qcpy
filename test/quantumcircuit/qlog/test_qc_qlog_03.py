import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z")
    for i in range(x):
        qc.h(i)
    for i in range(1, x):
        qc.cx(i - 1, i)
    for i in range(x):
        qc.h(i)


def test_01a():
    inc(1)


def test_01b():
    inc(2)


def test_01c():
    inc(3)


def test_01d():
    inc(4)
