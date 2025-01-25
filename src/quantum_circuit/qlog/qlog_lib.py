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
qlog_cross.qlog_dump_content.argtypes = [ctypes.POINTER(qlog_def), ctypes.c_bool]


def convert_qubits_qlog_append(qubits_to_apply):
    return (ctypes.c_ubyte * len(qubits_to_apply))(*qubits_to_apply)


<<<<<<< HEAD
qlog_cross.qlog_combine_qlogs.argtypes = [
    ctypes.POINTER(qlog_def),
    ctypes.POINTER(qlog_def),
]
qlog_cross.qlog_combine_qlogs.restype = ctypes.POINTER(qlog_def)
qlog_cross.qlog_compare_qlogs.argtypes = [
    ctypes.POINTER(qlog_def),
    ctypes.POINTER(qlog_def),
]
qlog_cross.qlog_compare_qlogs.restype = ctypes.POINTER(qlog_def)
=======
qlog_cross.qlog_dump_content.argtypes = [ctypes.POINTER(qlog_def), ctypes.c_bool]

qlog_cross.qlog_get_gate_names.argtypes = [ctypes.POINTER(qlog_def)]
qlog_cross.qlog_get_gate_names.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_char))

qlog_cross.qlog_get_gate_types.argtypes = [ctypes.POINTER(qlog_def)]
qlog_cross.qlog_get_gate_types.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_char))

qlog_cross.qlog_get_gate_qubits.argtypes = [ctypes.POINTER(qlog_def)]
qlog_cross.qlog_get_gate_qubits.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_uint8))

qlog_cross.qlog_get_entry_sizes.argtypes = [ctypes.POINTER(qlog_def)]
qlog_cross.qlog_get_entry_sizes.restype = ctypes.POINTER(ctypes.c_uint8)


def convert_arr_to_str(qlog_str_out, size):
    arr_of_strs = []
    for i in range(size):
        item = qlog_str_out[i]
        if item:
            arr_of_strs.append(ctypes.cast(item, ctypes.c_char_p).value.decode("utf-8"))
    return arr_of_strs


def convert_arr_to_int(qlog_int_out, size, sizes):
    arr_of_ints = []
    for i in range(size):
        item = qlog_int_out[i]
        if item:
            j = 0
            internal_ints = []
            while j < sizes[i]:
                internal_ints.append(item[j])
                j += 1
            arr_of_ints.append(internal_ints)
    return arr_of_ints


def convert_arr_to_sizes(qlog_int_out, size):
    arr_of_sizes = []
    for i in range(size):
        item = qlog_int_out[i]
        if item:
            arr_of_sizes.append(item)
    return arr_of_sizes
>>>>>>> main
