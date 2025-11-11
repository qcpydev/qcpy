# Visualization Guide

## Overview
QCPY includes built-in visualization tools to help users understand and analyze quantum circuits. This guide covers how to visualize circuits, state vectors, and Q-sphere representations.

## Visualizing Quantum Circuits
To display a circuit diagram:

```python
from qcpy import QuantumCircuit, visualize

qc = QuantumCircuit(qubits=3)
qc.h(0)
qc.cx(0, 1)
qc.cz(1, 2)
```

## Q-Sphere Representation
The Q-Sphere visualization shows quantum states on the Bloch sphere in a spherical format.

```python
visualize.qsphere(qc)
```

## Bloch Sphere Representation
To visualize individual qubits on the Bloch sphere:

```python
visualize.bloch(qc)  # Visualizes the bloch sphere
```

## State Vector Representation
To view the probability amplitudes of the quantum state:

```python
visualize.statevector(qc)
```

For further details, refer to the [Usage Guide](usage.md).

