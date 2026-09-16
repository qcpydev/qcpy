import numpy as np
from qcpy import quantumcircuit


def inc(x, n):
    qc = quantumcircuit(qubits=x, prep="z")
    for i in range(x):
        for _ in range(n):
            qc.i(i)


def test_03a():
    inc(5, 10)


def test_03b():
    inc(5, 20)


def test_03c():
    inc(5, 30)
