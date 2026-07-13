from typing import List
import ctypes
from typing import List
from .port_entry import Block, Block_Type, IMPORT_MAX_SIZE
import subprocess
from functools import reduce
from operator import or_


class Connect:
    def __init__(self, bootargs: List[str], qcpy_connect: str, qcpy_core: str, quack_core: str, quack_gpu_core: str):
        self.qcpy_core_bin_name = qcpy_core
        self.quack_core_bin_name = quack_core
        self.quack_gpu_core_bin_name = quack_gpu_core

        self.qcpy_connect = ctypes.CDLL(qcpy_connect)
        self.bootargs = [self.qcpy_core_bin_name, self.quack_core_bin_name, self.quack_gpu_core_bin_name] + bootargs
        self.gpu_enabled = True

        self.quantum_circuit_count = 0
        '''
        try:
            subprocess.check_output(["nvcc", "--version"]).decode()
        except FileNotFoundError:
            self.gpu_enabled = False
        '''
        self.bootargs += "Y" if self.gpu_enabled else "N"

        self.qcpy_connect.qcpy_boot_connect.restype = ctypes.c_int
        self.qcpy_connect.qcpy_boot_connect.argtypes = [ctypes.POINTER(ctypes.c_char_p)]

        encoded_args = [s.encode("utf-8") for s in self.bootargs]
        c_array = (ctypes.c_char_p * len(encoded_args))()

        for i, param in enumerate(encoded_args):
            c_array[i] = param
        self.qcpy_connect.qcpy_boot_connect(c_array)

        self.qcpy_connect.dock_add.restype = ctypes.c_int
        self.qcpy_connect.dock_add.argtypes = [ctypes.POINTER(Block)]

        self.qcpy_connect.dock_get_qc_state.argtypes = [ctypes.c_int]

        self.qcpy_connect.dock_get_qc_entries.argtypes = [
            ctypes.c_int,
            ctypes.POINTER(Block),
        ]
        self.qcpy_connect.dock_get_qc_entries.restype = ctypes.c_int

    def __create_qubit_bitmask__(self, to_bitmask: List[int]) -> int:
        return reduce(or_, (1 << i for i in to_bitmask),)

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

    def unpack_qubit_bitpack(self, to_decode: int, size: int) -> List[int]:
        return [(to_decode >> (i * 6)) & 0x3F for i in range(size)]

    def unpack_qubit_bitmask(self, to_decode: int) -> List[int]:
        res = []
        bitmask = 0
        qubit = 0
        while bitmask < (1 << 64):
            if to_decode & bitmask:
                res.append((qubit - 1))

            bitmask = 0
            bitmask |= 1 << qubit
            qubit += 1

        return res

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

        new_block = Block()
        new_block.qubit_bitmask = self.__create_qubit_bitmask__(qubits)

        if (controlled_qubits):
            new_block.controlled_bitmask = self.__create_qubit_bitmask__(controlled_qubits)
            new_block.controlled_bitpack = self.__create_qubit_bitpack__(controlled_qubits)

        if (target_qubits):
            new_block.target_bitmask = self.__create_qubit_bitmask__(target_qubits)
            new_block.target_bitpack = self.__create_qubit_bitpack__(target_qubits)

        new_block.type = Block_Type.QLOG_ENTRY
        new_block.reg = reg
        new_block.qubits = num_qubits
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

        temp_count = self.quantum_circuit_count
        self.quantum_circuit_count += 1
        return temp_count

    def get_quantum_circuit_state(self, reg: int, is_print: bool = False) -> None:
        self.qcpy_connect.dock_get_qc_state(reg)

    def get_gates_for_circuit(self, reg: int):
        entries = []
        sub_entries = (Block * IMPORT_MAX_SIZE)()

        output = self.qcpy_connect.dock_get_qc_entries(reg, sub_entries)
        entries += sub_entries
        """
        while (output == IMPORT_MAX_SIZE):
            count += output
            output = self.qcpy_connect.dock_get_qc_entries(reg, sub_entries)
            entries += sub_entries
            print("looping")
        """

        return entries
