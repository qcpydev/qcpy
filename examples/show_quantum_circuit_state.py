from qcpy import quantumcircuit

qc = quantumcircuit(qubits=4)
qc.h(0)
qc.h(1)
print(qc.state)
