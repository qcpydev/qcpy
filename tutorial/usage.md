# Usage Guide

## Creating a Quantum Circuit
To create a quantum circuit in QCPY, use the `QuantumCircuit` class:

```python
from qcpy import QuantumCircuit

qc = QuantumCircuit(qubits=3)
qc.h(0)  # Apply Hadamard gate to qubit 0
qc.cx(0, 1)  # Apply CNOT gate with control 0 and target 1
print(qc.state)  # Print quantum state
```

## Applying Quantum Gates
QCPY supports various quantum gates:

### **Single-Qubit Gates**
- **Hadamard (H)**: Creates superposition
- **Pauli-X (X)**: Flips the qubit state
- **Pauli-Y (Y)** and **Pauli-Z (Z)**: Phase shift operations

```python
qc.h(0)
qc.x(1)
qc.z(2)
```

### **Controlled Gates**
Controlled operations apply a gate based on the state of another qubit:

```python
qc.cx(0, 1)  # CNOT (Controlled-X)
qc.cz(1, 2)  # Controlled-Z
```

### **Parameterized Rotation Gates**
Rotation gates allow fine-tuned qubit transformations:

```python
import numpy as np
qc.rx(0, np.pi / 2)  # Rotate around X-axis
qc.ry(1, np.pi / 2)  # Rotate around Y-axis
qc.rz(2, np.pi / 2)  # Rotate around Z-axis
```

## Measuring a Circuit
To measure the quantum circuit, use:

```python
qc.measure()
```

## Running Simulations
To execute the quantum circuit and get results:

```python
results = qc.run(shots=1024)
print(results)
```

For more details on visualization, refer to the [Visualization Guide](visualization.md).

