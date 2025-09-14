from qcpy import quantumcircuit, amplitude

qc = quantumcircuit(qubits=4, big_endian=False)

qc.h(3)
qc.x(0)
qc.x(3)
print(amplitude(qc))
