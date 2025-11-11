from typing import List
import ctypes
from .port_entry import PortEntry
from enum import IntEnum
import time

class CxtRequestsTypes(IntEnum):
    QLOG_ENTRY = 0

class CxtItem(ctypes.Union):
    _fields_ = [
        ("qlog_register", ctypes.c_uint64),
        ("gate_type", ctypes.c_uint64),
        ("gate_name", ctypes.c_uint64),
        ("qubits", ctypes.c_uint64),
        ("qubit_cnt", ctypes.c_uint64),
        ("theta", ctypes.c_float),
        ("phi", ctypes.c_float),
        ("lambda", ctypes.c_float),
    ]

class CxtRequest(ctypes.Structure):
    _fields_ = [
        ("item", CxtItem),
        ("type", ctypes.c_int)
    ]

class Connect:
    def __init__(self, bootargs: List[str], qcpy_connect: str, qcpy_core: str):
        self.qcpy_core_bin_name = qcpy_core
        self.qcpy_connect = ctypes.CDLL(qcpy_connect)
        self.bootargs = [self.qcpy_core_bin_name] + bootargs

        self.qcpy_connect.qcpy_boot_connect.restype = ctypes.c_int
        self.qcpy_connect.qcpy_boot_connect.argtypes = [ctypes.POINTER(ctypes.c_char_p)]

        encoded_args = [s.encode('utf-8') for s in self.bootargs]
        c_array = (ctypes.c_char_p * len(encoded_args))()

        # Populate the C array with encoded strings
        for i, param in enumerate(encoded_args):
            c_array[i] = param

        boot_success = self.qcpy_connect.qcpy_boot_connect(c_array)

        #self.qcpy_connect.port_create_cxt_request.restype = ctypes.c_int
        #self.qcpy_connect.port_create_cxt_request.argtypes = [ctypes.POINTER(CxtRequest)]
        '''
        if (boot_success):
            assert(0)

        count = 5000

        for i in range(count):
            cxt = CxtRequest()
            cxt.item.gate_name = 1
            cxt.item.gate_type = 2
            cxt.type = CxtRequestsTypes.QLOG_ENTRY
            cxt.item.qlog_register = i

            self.qcpy_connect.port_create_cxt_request(ctypes.byref(cxt))

            self.qcpy_connect.port_import_queue_size.restype = ctypes.c_int
            size = self.qcpy_connect.port_import_queue_size()

            if (size > 15):
                raise ValueError(str(i), str(size))

            print(size)
        '''

    def add_entry(self, action):
        pass
