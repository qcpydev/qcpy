# Introduction & Prerequisites

## What is Quantum Computing?
Quantum computing is a different field of Computer Science that leverages the principles of quantum mechanics to process information in ways that classical computers  have difficulties with. Instead of using classical bits (0 or 1), quantum computers use **qubits**, which can exist in **superposition**—meaning they can represent both 0 and 1 at the same time.

## Understanding Qubits
A qubit is the most primitive unit of quantum information. Unlike classical bits, qubits take advantage of:
- **Superposition**: A qubit can be in a state of both 0 and 1 simultaneously.
- **Entanglement**: Qubits can become correlated, meaning the state of one qubit depends on another, no matter the distance between them.
- **Measurement**: When measured, a qubit collapses into either a 0 or a 1 with a certain probability.

## Quantum Gates and Operations
Quantum gates manipulate qubits in ways that enable quantum algorithms. Here are some fundamental quantum gates:

### **Pauli Gates**
- **X (Pauli-X)**: Equivalent to a NOT gate, flipping |0⟩ to |1⟩ and vice versa.

  **Matrix representation:**
  ```
  X = [0  1]
      [1  0]
  ```
  **Effect on a qubit state:**
  ```
  X |0⟩ = |1⟩, X |1⟩ = |0⟩
  ```

- **Y (Pauli-Y)**: Introduces a phase shift and flips the qubit.

  **Matrix representation:**
  ```
  Y = [ 0 -i]
      [ i  0]
  ```
  **Effect on a qubit state:**
  ```
  Y |0⟩ = i|1⟩, Y |1⟩ = -i|0⟩
  ```

- **Z (Pauli-Z)**: Introduces a phase shift without flipping the qubit.

  **Matrix representation:**
  ```
  Z = [1  0]
      [0 -1]
  ```
  **Effect on a qubit state:**
  ```
  Z |0⟩ = |0⟩, Z |1⟩ = -|1⟩
  ```

### **Hadamard Gate (H)**
Creates superposition by transforming |0⟩ into an equal mix of |0⟩ and |1⟩:

  **Matrix representation:**
  ```
  H = (1/sqrt(2)) * [ 1  1]
                     [ 1 -1]
  ```
  **Effect on a qubit state:**
  ```
  H |0⟩ = (|0⟩ + |1⟩) / sqrt(2)
  H |1⟩ = (|0⟩ - |1⟩) / sqrt(2)
  ```

```python
from qcpy import QuantumCircuit
qc = QuantumCircuit(1)
qc.h(0)
```

### **CNOT Gate (CX)**
A controlled operation that flips the target qubit if the control qubit is in state |1⟩.

  **Matrix representation:**
  ```
  CX = [1  0  0  0]
       [0  1  0  0]
       [0  0  0  1]
       [0  0  1  0]
  ```

```python
qc.cx(0, 1)
```

### **Phase Gates**
- **S Gate**: Introduces a 90-degree phase shift.

  **Matrix representation:**
  ```
  S = [1  0]
      [0  i]
  ```

- **T Gate**: Introduces a 45-degree phase shift.

  **Matrix representation:**
  ```
  T = [1       0]
      [0  e^(iπ/4)]
  ```

### **Rotation Gates**
- **RX, RY, RZ**: Rotate the qubit state around the X, Y, and Z axes.

  **Matrix representation of RZ:**
  ```
  RZ(θ) = [e^(-iθ/2)   0]
          [0   e^(iθ/2)]
  ```
  **Effect on a qubit:**
  ```
  RZ(θ) |0⟩ = e^(-iθ/2) |0⟩
  RZ(θ) |1⟩ = e^(iθ/2) |1⟩
  ```

For more on quantum gates, see the [Quantum Gates Reference](gates.md).
