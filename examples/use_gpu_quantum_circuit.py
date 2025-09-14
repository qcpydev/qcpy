from qcpy import quantumcircuit

qc = quantumcircuit(qubits=4, gpu=True)

qc.h(0)
qc.h(1)
