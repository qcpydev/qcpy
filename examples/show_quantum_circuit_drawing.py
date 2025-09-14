from qcpy import quantumcircuit


qc = quantumcircuit(qubits=5, big_endian=True)

qc.h([i for i in range(5)])
qc.cnot(0, 4)
print(qc)
