import subprocess
import numpy as np
from ...connect.boot import qcpy_connect
from ...connect.port_entry import QuantumGates
from ..cd import CircuitDrawing
from ..error import InvalidQubitPrepError, OutOfRangeError


class QuantumCircuit:
    """
    Organizes and handles inputs to multiple qubits, which architecture matrices are
        calculated on, and multiple class functionality that improve user output.

     Args:
         qubits (int): Number of qubits.
         endian (endian): Flag to set the state of the quantum circuit have big
                            endianess.
         prep (chr): to prep all qubits on construction of quantum circuit to be set at
                     a specific gate.
         sparse (bool): Flag to set if the quantum circuit should have it's matrices in sparse.
         gpu (bool): Flag to set if the quantum circuit should run in a GPU process.

     Attributes:
         calculator (GpuSparseCalculator/SparseCalculator/GpuCalculator/BaseCalculator): Sets itself
                     as a class depending on what architecure and improvement algorithms the user
                     provided.
         sparse (bool): From the given flag if the state should use sparse matrices.
         gpu (bool): From the given flag if the calculations should be ran on a GPU arch.
    """

    def __init__(
        self,
        qubits: int,
        endian: str = "",
        prep: str = "z",
        sparse: bool = False,
        gpu: bool = False,
    ):
        self.reg = -1
        self.sparse = sparse
        self.gpu = gpu
        self.qubits = qubits
        self.gpu = gpu
        self.endian = "big"

        if endian and endian != "big" or endian != "little":
            pass
            # raise endian error

        if prep != "z" and prep != "y" and prep != "x":
            raise InvalidQubitPrepError("Qubit prep is not x,y,or z")

        self.reg = qcpy_connect.add_quantum_circuit(self.gpu)

    def __eq__(self, circuit) -> bool:
        """Determines if the states of a given QC and this current one are equal to each other.
        Args:
            gate (str): Character symbol for the target drawing.
            control (int): Control qubit.
            target (int): Target qubit.
        Returns:
            bool: Boolean determinant if this is true or false.
        """
        pass

    def __add__(self, circuit) -> None:
        """Combine two quantum circuits together.
        Returns:
            None.
        """
        pass

    def __str__(self) -> str:
        """Prints out a circuit drawing in console.
        Returns:
            str: String output of the quantum circuit drawing.
        """
        return ""

    def __len__(self) -> int:
        """Amount of possible states in the quantum circuit (2^num_qubits).
        Returns:
            int: Number of possible output states.
        """
        return 0

    def set(self, circuit) -> None:
        """Sets the current state to a given one.
        Args:
            circuit (QuantumCircuit): The given state to set to the current one.
        """
        pass

    @property
    def state(self):
        """Get the state of the quantum circuit."""
        return

    @property
    def size(self) -> int:
        """How many qubits are in the quantum circuit.
        Returns:
            int: Number of qubits.
        """
        return 0

    def i(self, qubits_to_apply) -> None:
        """Use the identity gate on the quantum circuit.
        Args:
            qubits_to_apply (int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.IDENTITY,
            size=self.qubits,
        )
        return

    def h(self, qubits_to_apply) -> None:
        """Use the hadamard gate on the quantum circuit.
        Args:
            qubits_to_apply (int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.HADAMARD,
            size=self.qubits,
        )
        return

    def x(self, qubits_to_apply) -> None:
        """Use the Paulix gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.PAULIX,
            size=self.qubits,
        )

    def y(self, qubits_to_apply) -> None:
        """Use the Pauliy gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.PAULIY,
            size=self.qubits,
        )

    def z(self, qubits_to_apply) -> None:
        """Use the Pauliz gate on the quantum circuit.
        Args:
            qubit_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.PAULIZ,
            size=self.qubits,
        )

    def p(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the phase gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.PHASE,
            theta=theta,
            size=self.qubits,
        )

    def s(self, qubits_to_apply) -> None:
        """Use the S gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg, qubits=qubits_to_apply, gate=QuantumGates.S, size=self.qubits
        )

    def sdg(self, qubits_to_apply) -> None:
        """Use the Sdg gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.SDG,
            size=self.qubits,
        )

    def t(self, qubits_to_apply) -> None:
        """Use the T gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg, qubits=qubits_to_apply, gate=QuantumGates.T, size=self.qubits
        )

    def tdg(self, qubits_to_apply) -> None:
        """Use the Tdg gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.TDG,
            size=self.qubits,
        )

    def rz(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the Rz gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.RZ,
            theta=theta,
            size=self.qubits,
        )

    def ry(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the Ry gate on the quantum circuit.
        Args:
            qubits to apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.RY,
            theta=theta,
            size=self.qubits,
        )

    def rx(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the Rx gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.RX,
            theta=theta,
            size=self.qubits,
        )

    def sx(self, qubits_to_apply) -> None:
        """Use the Sx gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg, qubits=qubits_to_apply, gate=QuantumGates.SX, size=self.qubits
        )

    def sxdg(self, qubits_to_apply) -> None:
        """Use the Sxdg gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.SXDG,
            size=self.qubits,
        )

    def u(
        self,
        qubits_to_apply,
        theta: float = np.pi / 2,
        phi: float = np.pi / 2,
        lmbda: float = np.pi / 2,
    ) -> None:
        """Use the U gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=qubits_to_apply,
            gate=QuantumGates.U,
            size=self.qubits,
            theta=theta,
            phi=phi,
            lmbda=lmbda,
        )

    def custom(self, qubits_to_apply, gate: np.array) -> None:
        """Insert a custom gate into the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """

    def gatearray(self, gate_array) -> None:
        """Insert an array of gates into the quantum circuit.
        Args:
            circuit (QuantumCircuit): The given state to set to the current one.
        """

    def cx(self, control: int, target: int) -> None:
        """Insert a CX (CNot) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CX,
            size=self.qubits,
        )

    def ch(self, control: int, target: int) -> None:
        """Insert a CH (Controlled Hadamard) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CH,
            size=self.qubits,
        )

    def cy(self, control: int, target: int) -> None:
        """Insert a CY (Controlled Pauliy) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CY,
            size=self.qubits,
        )

    def cz(self, control: int, target: int) -> None:
        """Insert a CZ (Controlled Pauliz) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the target.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CZ,
            size=self.qubits,
        )

    def crx(self, control: int, target: int) -> None:
        """Insert a CRX (Controlled RX) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CRX,
            size=self.qubits,
        )

    def cry(self, control: int, target: int) -> None:
        """Insert a CRY (Controlled RY) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CRY,
            size=self.qubits,
        )

    def crz(self, control: int, target: int) -> None:
        """Insert a CRZ (Controlled RZ) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CRZ,
            size=self.qubits,
        )

    def cr1(self, control: int, target: int) -> None:
        """Insert a CR1 gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control, target],
            controlled_qubits=[control],
            target_qubits=[target],
            gate=QuantumGates.CR1,
            size=self.qubits,
        )

    def multicustom(self, gate: np.array, control: int, target: int) -> None:
        """Insert a custom controlled gate into the quantum circuit.
        Args:
            gate (np.array) gate to insert.
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """

    def ccx(self, control_one: int, control_two: int, target: int) -> None:
        """Insert a CCX (Toffoli) gate into the quantum circuit.
        Args:
            control_one (int): qubit to act as the control one for the gate.
            control_two (int): qubit to act as the control two for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control_one, control_two, target],
            controlled_qubits=[control_one, control_two],
            target_qubits=[target],
            gate=QuantumGates.CCX,
            size=self.qubits,
        )

    def qft(self, qubit_one: int, qubit_two: int, qubit_three: int) -> None:
        """Insert the Quantum Fourier Transform algorithm into the circuit.
        Args:
            qubit_one (int): qubit to act as the qubit one for the gate.
            qubit_two (int): qubit to act as the qubit two for the gate.
            qubit_three (int): qubit to act as the qubit three for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[qubit_one, qubit_two, qubit_three],
            gate=QuantumGates.QFT,
            size=self.qubits,
        )

    def rccx(self, control_one: int, control_two: int, target: int) -> None:
        """Insert a RCCX gate into the quantum circuit.
        Args:
            control_one (int): qubit to act as the control one for the gate.
            control_two (int): qubit to act as the control two for the gate.
            target (int): qubit to act as the target for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[control_one, control_two, target],
            controlled_qubits=[control_one, control_two],
            target_qubits=[target],
            gate=QuantumGates.RCCX,
            size=self.qubits,
        )

    def rc3x(self, qubit_1: int, qubit_2: int, qubit_3: int, qubit_4: int) -> None:
        """Insert a RC3X gate into the quantum circuit.
        Args:
            qubit_1 (int): qubit to act as the qubit one for the gate.
            qubit_2 (int): qubit to act as the qubit two for the gate.
            qubit_3 (int): qubit to act as the qubit three for the gate.
            qubit_4 (int): qubit to act as the qubit four for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[qubit_1, qubit_2, qubit_3, qubit_4],
            gate=QuantumGates.RC3X,
            size=self.qubits,
        )

    def swap(self, qubit_one: int, qubit_two: int) -> None:
        """Insert a SWAP gate into the quantum circuit.
        Args:
            qubit_one (int): qubit to act as the qubit one for the gate.
            qubit_two (int): qubit to act as the qubit two for the gate.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[qubit_one, qubit_two],
            gate=QuantumGates.SWAP,
            size=self.qubits,
        )

    def rxx(self, qubit_one: int, qubit_two: int, theta: float = np.pi / 2) -> None:
        """Insert a RXX gate into the quantum circuit.
        Args:
            qubit one (int): qubit to act as the qubit one for the gate.
            qubit two (int): qubit to act as the qubit two for the gate.
            theta (float): theta value to rotate the gate on the X axis.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[qubit_one, qubit_two],
            theta=theta,
            gate=QuantumGates.RXX,
            size=self.qubits,
        )

    def rzz(self, qubit_one: int, qubit_two: int, theta: float = np.pi / 2) -> None:
        """Insert a RZZ gate into the quantum circuit.
        Args:
            qubit_one (int): qubit to act as the qubit one for the gate.
            qubit_two (int): qubit to act as the qubit two for the gate.
            lmbda (float): The lambda value to rotate the gate on the z axis.
        """
        qcpy_connect.add_qg_entry(
            reg=self.reg,
            qubits=[qubit_one, qubit_two],
            theta=theta,
            gate=QuantumGates.RZZ,
            size=self.qubits,
        )
