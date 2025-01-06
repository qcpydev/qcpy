import ctypes
import os
from enum import IntEnum

FILE_PATH = os.path.dirname(os.path.realpath(__file__))
QLOG_BIN_FILE_PATH = (
    str(os.path.abspath(os.path.join(FILE_PATH, os.pardir, os.pardir))) + "/core.so"
)
qlog_cross = ctypes.CDLL(QLOG_BIN_FILE_PATH)


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
