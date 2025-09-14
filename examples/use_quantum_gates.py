from qcpy import quantumcircuit, gates

qc = quantumcircuit(qubits=4, big_endian=False)

qc.custom(gates.paulix(), 0)
print(qc)
