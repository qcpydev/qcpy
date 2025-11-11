# Quantum Gates Reference

## Overview
Quantum gates are the building blocks of quantum circuits. This section provides details on the gates available in QCPY.

## Single-Qubit Gates
These gates operate on a single qubit:

### **Identity Gate (I)**
Leaves the qubit unchanged.

```python
qc.i(0)
```

### **Pauli Gates**
- **X (NOT Gate)**: Flips |0⟩ to |1⟩ and vice versa.
- **Y**: Applies a 180-degree rotation around the Y-axis.
- **Z**: Applies a phase shift to |1⟩.

```python
qc.x(0)
qc.y(1)
qc.z(2)
```

### **Hadamard Gate (H)**
Creates superposition by transforming |0⟩ into an equal mix of |0⟩ and |1⟩.

```python
qc.h(0)
```

### **Phase Gates**
- **S Gate**: Introduces a 90-degree phase shift.
- **T Gate**: Introduces a 45-degree phase shift.

```python
qc.s(0)
qc.t(1)
```

### **Rotation Gates**
- **RX, RY, RZ**: Rotate the qubit state around the X, Y, and Z axes.

```python
qc.rx(0, np.pi / 2)
qc.ry(1, np.pi / 2)
qc.rz(2, np.pi / 2)
```

## Multi-Qubit Gates
These gates involve two or more qubits.

### **Controlled NOT (CNOT) Gate**
Flips the target qubit if the control qubit is |1⟩.

```python
qc.cx(0, 1)
```

### **Controlled-Z Gate (CZ)**
Applies a phase shift to |11⟩ state.

```python
qc.cz(1, 2)
```

### **SWAP Gate**
Swaps the states of two qubits.

```python
qc.swap(0, 1)
```

### **Toffoli Gate (CCX)**
A controlled-controlled-X gate.

```python
qc.ccx(0, 1, 2)
```

For more information on usage, visit the [Usage Guide](usage.md).

