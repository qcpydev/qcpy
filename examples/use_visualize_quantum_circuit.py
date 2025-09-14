from qcpy import quantumcircuit, visualize

qc = quantumcircuit(qubits=4)

qc.h([i for i in range(4)])

visualize.qsphere(qc)
