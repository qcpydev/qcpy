from qcpy import quantumcircuit, gates

SIZE = 4
gate_array = [gates.paulix() * SIZE]
qc = quantumcircuit(qubits=SIZE)
qc.gatearray(gate_array)
