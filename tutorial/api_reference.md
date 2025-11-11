# API Documentation

## Overview
This section provides an in-depth look at the functions and classes available in QCPY.

---

## QuantumCircuit Class
The `QuantumCircuit` class is used to create and manipulate quantum circuits.

### **Constructor**
```python
QuantumCircuit(qubits: int, big_endian: bool = False, prep: str = "z", sparse: bool = False, gpu: bool = False)
```
**Parameters:**
- `qubits` (int): Number of qubits in the circuit.
- `big_endian` (bool, optional): Endianness of the quantum state.
- `prep` (str, optional): Initial state preparation (`"x"`, `"y"`, or `"z"`).
- `sparse` (bool, optional): Whether to use sparse matrices.
- `gpu` (bool, optional): Enable GPU acceleration.

### **Methods**

#### **Gate Operations**
- `h(qubits)`: Apply Hadamard gate.
- `x(qubits)`: Apply Pauli-X gate.
- `y(qubits)`: Apply Pauli-Y gate.
- `z(qubits)`: Apply Pauli-Z gate.
- `cx(control, target)`: Apply CNOT gate.
- `cz(control, target)`: Apply controlled-Z gate.
- `swap(q1, q2)`: Swap two qubits.

#### **Parameterized Gates**
- `rx(qubits, theta)`: Rotate around X-axis by `theta`.
- `ry(qubits, theta)`: Rotate around Y-axis by `theta`.
- `rz(qubits, theta)`: Rotate around Z-axis by `theta`.

#### **Measurement & Execution**
- `measure()`: Measure all qubits.
- `run(shots=1024)`: Execute circuit and return measurement results.

#### **State Information**
- `state`: Returns the quantum state of the circuit.
- `size`: Returns the number of qubits.

For more details on specific functions, see the [Usage Guide](usage.md).

