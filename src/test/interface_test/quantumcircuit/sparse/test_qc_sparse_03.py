import numpy as np
from qcpy import quantumcircuit


def inc(x):
    qc = quantumcircuit(qubits=x, prep="z", sparse=True)
    for i in range(x):
        qc.h(i)
    for i in range(x):
        qc.t(i)
    return np.around(qc.state.flatten(), 3)
