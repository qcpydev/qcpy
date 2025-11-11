class InvalidGateInsertError(ValueError):
    """
    Value error for when a custom gate is sent into the quantumcircuit
    that is not a numpy array of complex types.

    Only raised within QuantumCircuit and is to make sure that the
    backend request can handle and conform the gate properly.

    Examples
    --------
    >>> from qcpy import quantumcircuit
    >>> import numpy as np
    >>> qc = quantumcircuit(qubits = 2)
    >>> insert = np.array([[1,1],[1,2]])
    >>> Error
    """


class NotUnitarySingleGateError(ValueError):
    """
    When a passed in possible gate to insert into the quantum circuit
    is of the right type, but is not in a unitary (2,2) shape.

    Examples
    --------
    >>> from qcpy import quantumcircuit
    >>> import numpy as np
    >>> qc = quantumcircuit(qubits = 2)
    >>> custom_gate = np.array([[1+0j],[1+0j],[1+0j]]
    >>> qc.custom(0, custom_gate)
    >>> Error
    """


class OutOfRangeError(IndexError):
    """
    When a insertion of a gate is out of bounds of the given quantum circuit.

    Examples
    --------
    >>> from qcpy import quatumcircuit
    >>> qc = quantumcircuit(qubits = 4)
    >>> qc.h(5)
    >>> Error
    """


class CudaNotInstalledWarning(Warning):
    """
    When the user tries to use the gpu flag and they do not have CUDA installed on
    their machine. qcpy will switch this flag back to false internally and inform the
    user they need to install CUDA

    Examples
    --------
    >>> from qcpy import quantumcircuit
    >>> qc = quantumcircuit(qubits = 2, gpu = True)
    >>> Warning
    """


class InvalidQubitPrepError(AttributeError):
    """
    When the user tries to prep the qubits within
    a quantum circuit with a bad value that is not
    x,y, or z.

    Examples
    --------
    >>> from qcpy import quantumcircuit
    >>> qc = quantumcircuit(qubits = 2, prep='u')
    >>> Error
    """


class ShowBitOutOfRangeError(TypeError):
    """
    When a user is using one of the
    provided tools, and they want to show
    a bit that is an integer or a binary string.
    However it is out of range of the given
    quantum circuit.

    Examples
    --------
    >>> from qcpy import quantumcircuit, amplitude
    >>> qc = quantumcircuit(qubits= 3)
    >>> print(amplitude(qc, show_bit = '1111'))
    >>> Error
    """


class InvalidSavePathError(NameError):
    """
    When the save path for the file is invalid
    for either the given operating system or
    in general file formatting.

    Examples
    -------
    >>> from qcpy import quantumcircuit, visualize
    >>> qc = quantumcircuit(qubits = 3)
    >>> visualize.amplitude(qc, save = '+===+___/')
    >>> Error
    """


class InvalidProbability(ValueError):
    """
    User has allowed outside external insertions that caused a
    collapse in the proability of the quantum circuit having a 100%
    probability, thus causing calculations error.

    Examples
    --------
    >>> from qcpy import quantumcircuit, probability
    >>> import numpy as np
    >>> qc = quantumcircuit(qubits = 3)
    >>> custom_insert = np.array([1+2j, 1+0j],[1+1j, 1+2j], 'F')
    >>> qc.custom(0, custom_insert)
    >>> probability(qc)
    >>> Error
    """


class BlochSphereOutOfRangeError(IndexError):
    """
    Bloch sphere visualizer only accepts formations
    of quantum circuits of one qubit to be accepted.

    Examples
    --------
    >>> from qcpy import quantumcircuit, visualize
    >>> qc = quantumcircuit(qubits = 3)
    >>> visualize.bloch(qc)
    >>> Error
    """


class QubitsToApplyInvalidTypeError(TypeError):
    """
    When a user tries inserting a gate using a
    value that is either not a integer or an array
    of integers.

    Examples
    --------
    >>> from qcpy import quantumcircuit
    >>> qc = quantumcircuit(qubits = 3)
    >>> qc.h('a')
    >>> Error
    """


class RoundBelowZeroError(ValueError):
    """
    When a user tries to round a phase angle, probability, etc.
    value from the tools and the rounding value is below 0.

    Examples
    --------
    >>> from qcpy import quantumcircuit, amplitude
    >>> qc = quantumcircuit(qubits = 3)
    >>> amplitude(qc, round = -1)
    >>> Error
    """
