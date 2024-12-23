import ctypes
import os
from enum import IntEnum

FILE_PATH = os.path.dirname(os.path.realpath(__file__))
QLOG_SO_FILE_PATH = str(FILE_PATH) + "/qlog_engine/qlog_engine.so"
qlog_cross = ctypes.CDLL(QLOG_SO_FILE_PATH)

qlog_gate_convert = {
    "IDENTITY": 0,
    "HADAMARD": 1,
    "PAULIX": 2,
    "PAULIY": 3,
    "PAULIZ": 4,
    "PHASE": 5,
    "S": 6,
    "SDG": 7,
    "T": 8,
    "TDG": 9,
    "RZ": 10,
    "RY": 11,
    "RX": 12,
    "SX": 13,
    "SXDG": 14,
    "U": 15,
    "CX": 16,
    "CH": 17,
    "CY": 18,
    "CZ": 19,
    "CRX": 20,
    "CRY": 21,
    "CRZ": 22,
    "CR1": 23,
    "CCX": 24,
    "QFT": 25,
    "RCCX": 26,
    "RC3X": 27,
    "SWAP": 28,
    "RXX": 29,
    "RZZ": 30,
    "CUSTOM": 31,
    "CUSTOMCONTROLLED": 32,
    "MULTI": 33,
    "CUSTOMBLOCK": 34,
    "CUSTOMALGORITHM": 35,
}

qlog_type_convert = {
    "SINGLE": 0,
    "CONTROLLED": 1,
    "MULTI": 2,
    "BLOCK": 3,
    "ALGORITHM": 4,
}


class qlog_stats_def(ctypes.Structure):
    _fields_ = [("test", ctypes.c_int)]


class qlog_entry_stats_def(ctypes.Structure):
    _fields = []


class qg_entry_gate(IntEnum):
    pass


class qg_entry_type(IntEnum):
    pass


class qlog_append_res(IntEnum):
    QLOG_APPEND_SUCCESS = 0
    QLOG_APPEND_FULL = 1
    QLOG_APPEND_ERROR = 2


class qlog_entry_def(ctypes.Structure):
    _fields_ = [
        ("qlog_entry_qubits", ctypes.POINTER(ctypes.c_uint8)),
        ("qlog_entry_qubit_cnt", ctypes.c_uint8),
        ("qlog_entry_gate", ctypes.c_int),
        ("qlog_entry_gate_type", ctypes.c_int),
        ("qlog_entry_stat", qlog_entry_stats_def),
    ]


class qlog_def(ctypes.Structure):
    _fields_ = [
        ("qlog_gate_cnt", ctypes.c_uint16),
        ("qlog_size", ctypes.c_uint16),
        ("qlog_entries", ctypes.POINTER(ctypes.POINTER(qlog_entry_def))),
        ("qlog_stat", qlog_stats_def),
    ]


qlog_cross.qlog_init.argtypes = [ctypes.c_uint8]
qlog_cross.qlog_init.restype = ctypes.POINTER(qlog_def)

qlog_cross.qlog_delete.argtypes = [ctypes.POINTER(qlog_def)]

qlog_cross.qlog_size.argtypes = [ctypes.POINTER(qlog_def)]
qlog_cross.qlog_size.restype = ctypes.c_uint16

qlog_cross.qlog_append.argtypes = [
    ctypes.POINTER(qlog_def),
    ctypes.POINTER(ctypes.c_uint8),
    ctypes.c_uint8,
    ctypes.c_int,
    ctypes.c_int,
]
qlog_cross.qlog_append.restype = qlog_append_res

qlog_cross.qlog_optimize_set.argtypes = [ctypes.POINTER(qlog_def)]
qlog_cross.qlog_optimize_set.restype = ctypes.POINTER(qlog_def)


def convert_qubits_qlog_append(qubits_to_apply):
    return (ctypes.c_ubyte * len(qubits_to_apply))(*qubits_to_apply)


qlog_cross.qlog_dump_content.argtypes = [ctypes.POINTER(qlog_def), ctypes.c_bool]
