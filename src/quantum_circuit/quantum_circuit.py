import subprocess
from .qlog.qlog import QLog
from .qlog.qlog_lib import qlog_gate_convert, qlog_type_convert
import numpy as np
from ..quantum_gate import (
    hadamard,
    identity,
    paulix,
    pauliy,
    pauliz,
    phase,
    r1,
    rx,
    ry,
    rz,
    s,
    sdg,
    sx,
    sxdg,
    t,
    tdg,
    u,
)

from ..circuit_drawing import CircuitDrawing
from ..errors import InvalidQubitPrepError, OutOfRangeError
from .base import BaseCalculator
from .gpu import GpuCalculator
from .gpu_sparse import GpuSparseCalculator
from .sparse import SparseCalculator


class QuantumCircuit:
    """Organizes and handles inputs to multiple qubits, which architecture matrices are
       calculated on, and multiple class functionality that improve user output.

    Args:
        qubits (int): Number of qubits.
        big_endian (bool): Flag to set the state of the quantum circuit have big
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
        big_endian: bool = False,
        prep: chr = "z",
        sparse: bool = False,
        gpu: bool = False,
    ):
        self.qlog = QLog(qubits)
        self.calculator = None
        self.sparse = sparse
        self.gpu = gpu
        if prep != "z" and prep != "y" and prep != "x":
            raise InvalidQubitPrepError("Qubit prep is not x,y,or z")
        if self.gpu:
            try:
                subprocess.check_output(["nvcc", "--version"]).decode()
            except FileNotFoundError:
                self.gpu = False
        if self.sparse and self.gpu:
            self.calculator = GpuSparseCalculator(qubits, big_endian, prep)
        elif self.sparse:
            self.calculator = SparseCalculator(qubits, big_endian, prep)
        elif self.gpu:
            self.calculator = GpuCalculator(qubits, big_endian, prep)
        else:
            self.calculator = BaseCalculator(qubits, big_endian, prep)
        self.circuit_drawing = CircuitDrawing(qubits)

    def __eq__(self, circuit) -> bool:
        """Determines if the states of a given QC and this current one are equal to each other.
        Args:
            gate (str): Character symbol for the target drawing.
            control (int): Control qubit.
            target (int): Target qubit.
        Returns:
            bool: Boolean determinant if this is true or false.
        """
        temp_state = self.calculator.state
        foreign_temp = circuit.calculator.state
        if self.sparse:
            temp_state = temp_state.toarray()
        if circuit.sparse:
            foreign_temp = foreign_temp.toarray()
        return (temp_state == foreign_temp).all()

    def __str__(self) -> str:
        """Prints out a circuit drawing in console.
        Returns:
            str: String output of the quantum circuit drawing.
        """
        return self.circuit_drawing.make()

    def __len__(self) -> int:
        """Amount of possible states in the quantum circuit (2^num_qubits).
        Returns:
            int: Number of possible output states.
        """
        return 2**self.calculator.qubits

    def __add_single_drawing__(self, qubits_to_apply, gate: str) -> None:
        """Port to send gate information to the CircuitDrawing class.
        Args:
            qubits_to_apply (int/List[int]): Sets either an array or a single value to set the gate string at.
            gate (str): The character representation of the gate.
        """
        if not isinstance(qubits_to_apply, int):
            for qubit in qubits_to_apply:
                if qubit not in range(self.calculator.qubits):
                    OutOfRangeError("qubit is out of range of size of quantum circuit")
                self.circuit_drawing.insert_single(gate, qubit)
        elif isinstance(qubits_to_apply, int):
            if qubits_to_apply not in range(self.calculator.qubits):
                OutOfRangeError("qubit is out of range of size of quantum circuit")
            self.circuit_drawing.insert_single(gate, qubits_to_apply)

    def __add_qlog_item__(
        self, qubits_to_apply, gate_name: str, gate_type: str
    ) -> None:
        if isinstance(qubits_to_apply, int):
            qubits_to_apply = [qubits_to_apply]
        elif gate_type == "SINGLE":
            print(qubits_to_apply)
            for i in range(len(qubits_to_apply)):
                self.__add_qlog_item__(qubits_to_apply[i], gate_name, gate_type)
            return
        gate_val = qlog_gate_convert[gate_name]
        type_val = qlog_type_convert[gate_type]
        num_qubits = len(qubits_to_apply)
        self.qlog.append(qubits_to_apply, num_qubits, type_val, gate_val)

    def set(self, circuit) -> None:
        """Sets the current state to a given one.
        Args:
            circuit (QuantumCircuit): The given state to set to the current one.
        """
        self.calculator.state = circuit.calculator.state

    @property
    def state(self):
        """Get the state of the quantum circuit."""
        if not self.sparse and not self.gpu:
            temp_state = self.calculator.state
        if self.sparse and self.gpu:
            temp_state = self.calculator.state.get()
            temp_state = np.array(temp_state.toarray(), "F")
        if self.gpu and not self.sparse:
            temp_state = self.calculator.state.get()
        if self.sparse and not self.gpu:
            temp_state = np.array(self.calculator.state.toarray(), "F")
        return temp_state

    @property
    def size(self) -> int:
        """How many qubits are in the quantum circuit.
        Returns:
            int: Number of qubits.
        """
        return self.calculator.qubits

    def i(self, qubits_to_apply) -> None:
        """Use the identity gate on the quantum circuit.
        Args:
            qubits_to_apply (int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, identity())
        self.__add_single_drawing__(qubits_to_apply, "I")
        self.__add_qlog_item__(qubits_to_apply, "IDENTITY", "SINGLE")

    def h(self, qubits_to_apply) -> None:
        """Use the hadamard gate on the quantum circuit.
        Args:
            qubits_to_apply (int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, hadamard())
        self.__add_single_drawing__(qubits_to_apply, "H")
        self.__add_qlog_item__(qubits_to_apply, "HADAMARD", "SINGLE")

    def x(self, qubits_to_apply) -> None:
        """Use the Paulix gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, paulix())
        self.__add_single_drawing__(qubits_to_apply, "X")
        self.__add_qlog_item__(qubits_to_apply, "PAULIX", "SINGLE")

    def y(self, qubits_to_apply) -> None:
        """Use the Pauliy gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, pauliy())
        self.__add_single_drawing__(qubits_to_apply, "Y")
        self.__add_qlog_item__(qubits_to_apply, "PAULIY", "SINGLE")

    def z(self, qubits_to_apply) -> None:
        """Use the Pauliz gate on the quantum circuit.
        Args:
            qubit_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, pauliz())
        self.__add_single_drawing__(qubits_to_apply, "Z")
        self.__add_qlog_item__(qubits_to_apply, "PAULIZ", "SINGLE")

    def p(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the phase gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, phase(theta))
        self.__add_single_drawing__(qubits_to_apply, "P")
        self.__add_qlog_item__(qubits_to_apply, "PHASE", "SINGLE")

    def s(self, qubits_to_apply) -> None:
        """Use the S gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, s())
        self.__add_single_drawing__(qubits_to_apply, "S")
        self.__add_qlog_item__(qubits_to_apply, "S", "SINGLE")

    def sdg(self, qubits_to_apply) -> None:
        """Use the Sdg gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, sdg())
        self.__add_single_drawing__(qubits_to_apply, "S†")
        self.__add_qlog_item__(qubits_to_apply, "SDG", "SINGLE")

    def t(self, qubits_to_apply) -> None:
        """Use the T gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, t())
        self.__add_single_drawing__(qubits_to_apply, "T")
        self.__add_qlog_item__(qubits_to_apply, "T", "SINGLE")

    def tdg(self, qubits_to_apply) -> None:
        """Use the Tdg gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, tdg())
        self.__add_single_drawing__(qubits_to_apply, "T†")
        self.__add_qlog_item__(qubits_to_apply, "TDG", "SINGLE")

    def rz(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the Rz gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, rz(theta))
        self.__add_single_drawing__(qubits_to_apply, "RZ")
        self.__add_qlog_item__(qubits_to_apply, "RZ", "SINGLE")

    def ry(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the Ry gate on the quantum circuit.
        Args:
            qubits to apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, ry(theta))
        self.__add_single_drawing__(qubits_to_apply, "RY")
        self.__add_qlog_item__(qubits_to_apply, "RY", "SINGLE")

    def rx(self, qubits_to_apply, theta: float = np.pi / 2) -> None:
        """Use the Rx gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, rx(theta))
        self.__add_single_drawing__(qubits_to_apply, "RX")
        self.__add_qlog_item__(qubits_to_apply, "RX", "SINGLE")

    def sx(self, qubits_to_apply) -> None:
        """Use the Sx gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, sx())
        self.__add_single_drawing__(qubits_to_apply, "SX")
        self.__add_qlog_item__(qubits_to_apply, "SX", "SINGLE")

    def sxdg(self, qubits_to_apply) -> None:
        """Use the Sxdg gate on the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, sxdg())
        self.__add_single_drawing__(qubits_to_apply, "SX†")
        self.__add_qlog_item__(qubits_to_apply, "SXDG", "SINGLE")

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
        self.calculator.pass_single_gate(qubits_to_apply, u(theta, phi, lmbda))
        self.__add_single_drawing__(qubits_to_apply, "U")
        self.__add_qlog_item__(qubits_to_apply, "U", "SINGLE")

    def custom(self, qubits_to_apply, gate: np.array) -> None:
        """Insert a custom gate into the quantum circuit.
        Args:
            qubits_to_apply(int, arr[int]): qubits to apply the gate to.
        """
        self.calculator.pass_single_gate(qubits_to_apply, gate)
        self.__add_single_drawing__(qubits_to_apply, "C")
        self.__add_qlog_item__(qubits_to_apply, "CUSTOM", "SINGLE")

    def gatearray(self, gate_array) -> None:
        """Insert an array of gates into the quantum circuit.
        Args:
            circuit (QuantumCircuit): The given state to set to the current one.
        """
        self.calculator.pass_custom_gate_queue(gate_array)

    def cx(self, control: int, target: int) -> None:
        """Insert a CX (CNot) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control, target, paulix())
        self.circuit_drawing.add_control("X", control, target)
        self.__add_qlog_item__([control, target], "CX", "CONTROLLED")

    def ch(self, control: int, target: int) -> None:
        """Insert a CH (Controlled Hadamard) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control, target, hadamard())
        self.circuit_drawing.add_control("H", control, target)
        self.__add_qlog_item__(qubits_to_apply, "CH", "CONTROLLED")

    def cy(self, control: int, target: int) -> None:
        """Insert a CY (Controlled Pauliy) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
        """
        self.calculator.pass_multi_gate(control, target, pauliy())
        self.circuit_drawing.add_control("Y", control, target)
        self.__add_qlog_item__(qubits_to_apply, "CY", "CONTROLLED")

    def cz(self, control: int, target: int) -> None:
        """Insert a CZ (Controlled Pauliz) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the target.
        """
        self.calculator.pass_multi_gate(control, target, pauliz())
        self.circuit_drawing.add_control("Z", control, target)
        self.__add_qlog_item__(qubits_to_apply, "CZ", "CONTROLLED")

    def crx(self, control: int, target: int) -> None:
        """Insert a CRX (Controlled RX) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control, target, rx())
        self.circuit_drawing.add_control("RX", control, target)
        self.__add_qlog_item__(qubits_to_apply, "CRX", "CONTROLLED")

    def cry(self, control: int, target: int) -> None:
        """Insert a CRY (Controlled RY) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control, target, ry())
        self.circuit_drawing.add_control("RY", control, target)
        self.__add_qlog_item__(qubits_to_apply, "CRY", "CONTROLLED")

    def crz(self, control: int, target: int) -> None:
        """Insert a CRZ (Controlled RZ) gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control, target, rz())
        self.circuit_drawing.add_control("RZ", control, target)
        self.__add_qlog_item__([control, target], "CRZ", "CONTROLLED")

    def cr1(self, control: int, target: int) -> None:
        """Insert a CR1 gate into the quantum circuit.
        Args:
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control, target, r1())
        self.circuit_drawing.add_control("R1", control, target)
        self.__add_qlog_item__([control, target], "CR1", "CONTROLLED")

    def multicustom(self, gate: np.array, control: int, target: int) -> None:
        """Insert a custom controlled gate into the quantum circuit.
        Args:
            gate (np.array) gate to insert.
            control (int): qubit to act as the control for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control, target, gate)
        self.circuit_drawing.add_control("C", control, target)
        self.__add_qlog_item__([control, target], "MULTICUSTOM", "CONTROLLED")

    def ccx(self, control_one: int, control_two: int, target: int) -> None:
        """Insert a CCX (Toffoli) gate into the quantum circuit.
        Args:
            control_one (int): qubit to act as the control one for the gate.
            control_two (int): qubit to act as the control two for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_multi_gate(control_two, target, sx())
        self.calculator.pass_multi_gate(control_one, control_two, paulix())
        self.calculator.pass_multi_gate(control_two, target, sxdg())
        self.calculator.pass_multi_gate(control_one, control_two, paulix())
        self.calculator.pass_multi_gate(control_one, target, sx())
        self.circuit_drawing.add_multi("X", [control_one, control_two], target)
        self.__add_qlog_item__([control_one, control_two, target], "CCX", "MULTI")

    def qft(self, qubit_one: int, qubit_two: int, qubit_three: int) -> None:
        """Insert the Quantum Fourier Transform algorithm into the circuit.
        Args:
            qubit_one (int): qubit to act as the qubit one for the gate.
            qubit_two (int): qubit to act as the qubit two for the gate.
            qubit_three (int): qubit to act as the qubit three for the gate.
        """
        self.calculator.pass_single_gate(qubit_one, hadamard())
        self.calculator.pass_multi_gate(qubit_two, qubit_one, r1())
        self.calculator.pass_multi_gate(qubit_two, qubit_three, r1())
        self.calculator.pass_single_gate(qubit_two, hadamard())
        self.calculator.pass_multi_gate(qubit_three, qubit_two, r1())
        self.calculator.pass_single_gate(qubit_three, hadamard())
        self.calculator.pass_multi_gate(qubit_one, qubit_three, paulix())
        self.calculator.pass_multi_gate(qubit_three, qubit_one, paulix())
        self.calculator.pass_multi_gate(qubit_one, qubit_three, paulix())
        self.circuit_drawing.add_block("QFT", [qubit_one, qubit_two, qubit_three])
        self.__add_qlog_item__([qubit_one, qubit_two, qubit_three], "QFT", "ALGORITHM")

    def rccx(self, control_one: int, control_two: int, target: int) -> None:
        """Insert a RCCX gate into the quantum circuit.
        Args:
            control_one (int): qubit to act as the control one for the gate.
            control_two (int): qubit to act as the control two for the gate.
            target (int): qubit to act as the target for the gate.
        """
        self.calculator.pass_single_gate(target, u(np.pi / 2, 0, np.pi))
        self.calculator.pass_single_gate(target, u(0, 0, np.pi / 4))
        self.calculator.pass_multi_gate(control_two, target, paulix())
        self.calculator.pass_single_gate(target, u(0, 0, (-1 * np.pi) / 4))
        self.calculator.pass_multi_gate(control_one, target, paulix())
        self.calculator.pass_single_gate(target, u(0, 0, np.pi / 4))
        self.calculator.pass_multi_gate(control_two, target, paulix())
        self.calculator.pass_single_gate(target, u(0, 0, (-1 * np.pi) / 4))
        self.calculator.pass_single_gate(target, u(np.pi / 2, 0, np.pi))
        self.circuit_drawing.add_block("RCCX", [control_one, control_two, target])
        self.__add_qlog_item__([control_one, control_two, target], "RCCX", "BLOCK")

    def rc3x(self, qubit_1: int, qubit_2: int, qubit_3: int, qubit_4: int) -> None:
        """Insert a RC3X gate into the quantum circuit.
        Args:
            qubit_1 (int): qubit to act as the qubit one for the gate.
            qubit_2 (int): qubit to act as the qubit two for the gate.
            qubit_3 (int): qubit to act as the qubit three for the gate.
            qubit_4 (int): qubit to act as the qubit four for the gate.
        """
        self.calculator.pass_single_gate(qubit_4, u(np.pi / 2, 0, np.pi))
        self.calculator.pass_single_gate(qubit_4, u(0, 0, np.pi / 4))
        self.calculator.pass_multi_gate(qubit_3, qubit_4, paulix())
        self.calculator.pass_single_gate(qubit_4, u(0, 0, (-1 * np.pi) / 4))
        self.calculator.pass_single_gate(qubit_4, u(np.pi / 2, 0, np.pi))
        self.calculator.pass_multi_gate(qubit_1, qubit_4, paulix())
        self.calculator.pass_single_gate(qubit_4, u(0, 0, np.pi / 4))
        self.calculator.pass_multi_gate(qubit_2, qubit_4, paulix())
        self.calculator.pass_single_gate(qubit_4, u(0, 0, (-1 * np.pi / 4)))
        self.calculator.pass_multi_gate(qubit_1, qubit_4, paulix())
        self.calculator.pass_single_gate(qubit_4, u(0, 0, np.pi / 4))
        self.calculator.pass_multi_gate(qubit_2, qubit_4, paulix())
        self.calculator.pass_single_gate(qubit_4, u(0, 0, (-1 * np.pi / 4)))
        self.calculator.pass_single_gate(qubit_4, u(np.pi / 2, 0, np.pi))
        self.calculator.pass_single_gate(qubit_4, u(0, 0, np.pi / 4))
        self.calculator.pass_multi_gate(qubit_3, qubit_4, paulix())
        self.calculator.pass_single_gate(qubit_4, u(0, 0, (-1 * np.pi / 4)))
        self.calculator.pass_single_gate(qubit_4, u(np.pi / 2, 0, np.pi))
        self.circuit_drawing.add_block("RC3X", [qubit_1, qubit_2, qubit_3, qubit_4])
        self.__add_qlog_item__([qubit_1, qubit_2, qubit_3, qubit_4], "RC3X", "BLOCK")

    def swap(self, qubit_one: int, qubit_two: int) -> None:
        """Insert a SWAP gate into the quantum circuit.
        Args:
            qubit_one (int): qubit to act as the qubit one for the gate.
            qubit_two (int): qubit to act as the qubit two for the gate.
        """
        self.calculator.pass_multi_gate(qubit_one, qubit_two, paulix())
        self.calculator.pass_multi_gate(qubit_two, qubit_one, paulix())
        self.calculator.pass_multi_gate(qubit_one, qubit_two, paulix())
        self.circuit_drawing.add_swap(qubit_one, qubit_two)
        self.__add_qlog_item__([qubit_one, qubit_two], "SWAP", "CONTROLLED")

    def rxx(self, qubit_one: int, qubit_two: int, theta: float = np.pi / 2) -> None:
        """Insert a RXX gate into the quantum circuit.
        Args:
            qubit one (int): qubit to act as the qubit one for the gate.
            qubit two (int): qubit to act as the qubit two for the gate.
            theta (float): theta value to rotate the gate on the X axis.
        """
        self.calculator.pass_single_gate(
            qubit_one, u(phi=theta, lmbda=0, theta=np.pi / 2)
        )
        self.calculator.pass_single_gate(qubit_two, hadamard())
        self.calculator.pass_multi_gate(qubit_one, qubit_two, paulix())
        self.calculator.pass_single_gate(qubit_two, u(-1 * theta, 0, 0))
        self.calculator.pass_multi_gate(qubit_one, qubit_two, paulix())
        self.calculator.pass_single_gate(
            qubit_one, u(np.pi / 2, -1 * np.pi, np.pi - theta)
        )
        self.calculator.pass_single_gate(qubit_two, hadamard())
        self.circuit_drawing.add_block("RXX", [qubit_one, qubit_two])
        self.__add_qlog_item__([qubit_one, qubit_two], "RXX", "CONTROLLED")

    def rzz(self, qubit_one: int, qubit_two: int, lmbda: float = np.pi / 2) -> None:
        """Insert a RZZ gate into the quantum circuit.
        Args:
            qubit_one (int): qubit to act as the qubit one for the gate.
            qubit_two (int): qubit to act as the qubit two for the gate.
            lmbda (float): The lambda value to rotate the gate on the z axis.
        """
        self.calculator.pass_multi_gate(qubit_one, qubit_two, paulix())
        self.calculator.pass_single_gate(qubit_two, u(0, lmbda, 0))
        self.calculator.pass_multi_gate(qubit_one, qubit_two, paulix())
        self.circuit_drawing.add_block("RZZ", [qubit_one, qubit_two])
        self.__add_qlog_item__([qubit_one, qubit_two], "RZZ", "CONTROLLED")
