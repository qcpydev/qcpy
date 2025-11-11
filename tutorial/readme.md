# qcpy - a quantum computing simulator

qcpy is an open-source python library designed for simulating, visualizing, and debugging quantum circuits. it is designed to be lightweight, easy to understand, and provide a robust range of customizable tools.

## documentation
this provides a high-level overview. for more information:

- [introduction & prerequisites](prerequisites.md)
- [installation guide](installation.md)
- [usage guide](usage.md)
- [quantum gates reference](gates.md)
- [api documentation](api_reference.md)
- [visualization guide](visualization.md)
- [first quantum circuit](tutorial.md)
- [deutsch-jozsa algorithm example](deutsch-jozsa.md)

---

## installation

to install qcpy:

```sh
pip install qcpydev
```

for more setup details: [installation](INSTALLATION.md).

---

## getting started:

to create a quantum circuit:

```python
from qcpy import quantumcircuit

qc = quantumcircuit(qubits=3)
qc.h(0)  # Apply Hadamard gate to qubit 0
qc.cx(0, 1)  # Apply CNOT gate with control 0 and target 1
print(qc.state)  # Print quantum state
```

for advanced examples, see: [usage guide](USAGE.md).

---

## learn quantum computing

new to quantum computing? start here: [introduction & prerequisites](prerequisites.md), this covers:
- what are qubits?
- basic quantum gates (hadamard, pauli, cnot, etc.)
- concepts of superposition and entanglement

explore: **[quantum gates reference](gates.md)** for a deeper understanding.

---

## visualizing quantum circuits

qcpy includes built-in visualization tools:

```python
from qcpy import quantumcircuit, visualize

qc = quantumcircuit(qubits=3)
qc.h([0, 1, 2])

visualize.qsphere(qc)  # Display Q-Sphere representation
```

more visualization methods: [visualization guide](visualization.md).

---

## contribution/community

see our [contributing guide](CONTRIBUTING.md) for more details

for questions/discussions/support: [discord](https://discord.gg/jWxYXFzraK).

---

## license

qcpy is released under the Apache 2.0 license. See the [license](LICENSE.md) for details.
