import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z")
    qc2 = quantumcircuit(qubits=x, prep="z")
    for i in range(x):
        qc.h(i)
        qc2.h(i)

    for i in range(x):
        qc.t(i)
        qc2.h(i)
