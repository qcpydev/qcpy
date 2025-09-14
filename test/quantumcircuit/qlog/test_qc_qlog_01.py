import numpy as np
from qcpy import quantumcircuit


def inc(x, n):
    qc = quantumcircuit(qubits=x, prep="z")
    for _ in range(n):
        qc.h([j for j in range(x)])

    for _ in range(n):
        qc.t([j for j in range(x)])

def test_01a():
    inc(1, 20)

def test_01b():
    inc(2, 30)

def test_01c():
    inc(3, 40)

def test_01d():
    inc(4, 50)

