from qcpy import quantumcircuit
import numpy as np


pauli_x = np.array([[0 + 0j, 1 + 0j], [1 + 0j, 0 + 0j]], "F")

qc = quantumcircuit(qubits=4, big_endian=True, prep="y")

qc.custom(3, pauli_x)
print(qc)
