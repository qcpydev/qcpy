from typing import List
import ctypes
import subprocess
import time
from typing import List
from .port_entry import Block, Block_Type


class Connect:
    def __init__(self, bootargs: List[str], qcpy_connect: str, qcpy_core: str):
        self.qcpy_core_bin_name = qcpy_core
        self.qcpy_connect = ctypes.CDLL(qcpy_connect)
        self.bootargs = [self.qcpy_core_bin_name] + bootargs
        self.gpu_enabled = True
        self.quantum_circuit_count = 0
        """
        try:
            subprocess.check_output(["nvcc", "--version"]).decode()
        except FileNotFoundError:
            self.gpu_enabled = False
        """
        self.qcpy_connect.qcpy_boot_connect.restype = ctypes.c_int
        self.qcpy_connect.qcpy_boot_connect.argtypes = [ctypes.POINTER(ctypes.c_char_p)]

        encoded_args = [s.encode("utf-8") for s in self.bootargs]
        c_array = (ctypes.c_char_p * len(encoded_args))()

        for i, param in enumerate(encoded_args):
            c_array[i] = param

        self.qcpy_connect.qcpy_boot_connect(c_array)
        self.qcpy_connect.dock_add.restype = ctypes.c_int
        self.qcpy_connect.dock_add.argtypes = [ctypes.POINTER(Block)]

        time.sleep(
            0.1
        )  # << what the, aight FUCK it I am gonna have a bool saying ready on the backend, set up await/while loop until thats true

    def __create_qubit_bitmask__(self, to_bitmask: List[int]) -> int:
        bitmask = 0

        for val in to_bitmask:
            if val >= 64:
                return -1

            bitmask |= 1 << val

        return bitmask

    def __create_qubit_bitpack__(self, to_bitpack: List[int]) -> int:
        bitpack = 0
        size = len(to_bitpack)

        if size >= 64:
            return -1

        for i in range(size):
            if to_bitpack[i] >= 64:
                return -1

            bitpack |= to_bitpack[i] << (i * 6)

        return bitpack

    def __unpack_qubit_bitpack__(self, to_decode: int, size: int) -> List[int]:
        return [(to_decode >> (i * 6)) & 0x3F for i in range(size)]

    def __validate_control_target_qubits__(
        self, control_list: List[int], target_list: List[int]
    ) -> bool:
        control_set = set(control_list)
        target_set = set(target_list)

        for control in control_set:
            if control in target_set:
                return False

        return True

    def add_qg_entry(
        self,
        reg: int,
        qubits: List[int],
        gate: int,
        size: int,
        controlled_qubits: List[int] = [],
        target_qubits: List[int] = [],
        theta: float = 0,
        phi: float = 0,
        lmbda: float = 0,
    ) -> None:
        if not self.__validate_control_target_qubits__(
            controlled_qubits, target_qubits
        ):
            return
        num_qubits = len(qubits)

        num_controlled = len(controlled_qubits)
        num_target = len(target_qubits)

        qubit_bitmask = self.__create_qubit_bitmask__(qubits)

        controlled_bitmask = self.__create_qubit_bitmask__(controlled_qubits)
        target_bitmask = self.__create_qubit_bitmask__(target_qubits)
        controlled_bitpack = self.__create_qubit_bitpack__(controlled_qubits)
        target_bitpack = self.__create_qubit_bitpack__(target_qubits)

        new_block = Block()

        new_block.type = Block_Type.QLOG_ENTRY
        new_block.reg = reg
        new_block.qubits = num_qubits

        new_block.qubit_bitmask = qubit_bitmask

        new_block.controlled_bitmask = controlled_bitmask
        new_block.controlled_bitpack = controlled_bitpack

        new_block.target_bitmask = target_bitmask
        new_block.target_bitpack = target_bitpack

        new_block.theta = theta
        new_block.phi = phi
        new_block.lmbda = lmbda
        new_block.gate = int(gate)
        new_block.size = size
        new_block.controlled_count = num_controlled
        new_block.target_count = num_target
        new_block.inverted = False
        new_block.big_endian = False

        self.qcpy_connect.dock_add(new_block)

    def add_quantum_circuit(self, attempt_using_gpu: bool = False) -> int:
        if attempt_using_gpu and not self.gpu_enabled:
            pass
            # raise failuire

        temp_count = self.quantum_circuit_count
        self.quantum_circuit_count += 1
        return temp_count
