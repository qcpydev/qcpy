from enum import IntEnum
import ctypes

IMPORT_MAX_SIZE = 64

class QuantumGates(IntEnum):
    IDENTITY = 0
    HADAMARD = 1
    PAULIX = 2
    PAULIY = 3
    PAULIZ = 4
    PHASE = 5
    S = 6
    SDG = 7
    T = 8
    TDG = 9
    RZ = 10
    RY = 11
    RX = 12
    SX = 13
    SXDG = 14
    U = 15
    CX = 16
    CH = 17
    CY = 18
    CZ = 19
    CRX = 20
    CRY = 21
    CRZ = 22
    CSX = 23
    CSXDG = 24
    CR1 = 25
    CCX = 26
    QFT = 27
    RCCX = 28
    RC3X = 29
    SWAP = 30
    RXX = 31
    RZZ = 32
    CUSTOM = 33
    CUSTOMCONTROLLED = 34
    MULTI = 35
    CUSTOMBLOCK = 36
    CUSTOMALGORITHM = 37
    MAX = 38


class Block_Type(IntEnum):
    QLOG_ENTRY = 0
    CLOG_ENTRY = 1

class Block(ctypes.Structure):
    _fields_ = [
        ("qubit_bitmask", ctypes.c_uint64),
        ("controlled_bitmask", ctypes.c_uint64),
        ("controlled_bitpack", ctypes.c_uint64),
        ("target_bitmask", ctypes.c_uint64),
        ("target_bitpack", ctypes.c_uint64),
        ("theta", ctypes.c_float),
        ("phi", ctypes.c_float),
        ("lmbda", ctypes.c_float),
        ("reg", ctypes.c_uint16),
        ("gate", ctypes.c_uint8, 8),
        ("qubits", ctypes.c_uint8, 8),
        ("size", ctypes.c_uint8, 8),
        ("controlled_count", ctypes.c_uint8, 8),
        ("target_count", ctypes.c_uint8, 8),
        ("type", ctypes.c_int8, 4),
        ("inverted", ctypes.c_bool, 1),
        ("big_endian", ctypes.c_bool, 1),
        ("used", ctypes.c_bool, 1),
    ]

