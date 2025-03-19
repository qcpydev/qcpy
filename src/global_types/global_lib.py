import ctypes
import os
from enum import IntEnum

FILE_PATH = os.path.dirname(os.path.realpath(__file__))
GLOBAL_BIN_FILE_PATH = str(FILE_PATH) + "/global.so"
global_gate_convert = {
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
    "CSX": 23,
    "CSXDG": 24,
    "CR1": 25,
    "CCX": 26,
    "QFT": 27,
    "RCCX": 28,
    "RC3X": 29,
    "SWAP": 30,
    "RXX": 31,
    "RZZ": 32,
    "CUSTOM": 33,
    "CUSTOMCONTROLLED": 34,
    "MULTI": 35,
    "CUSTOMBLOCK": 36,
    "CUSTOMALGORITHM": 37,
}

global_type_convert = {
    "SINGLE": 0,
    "CONTROLLED": 1,
    "MULTI": 2,
    "BLOCK": 3,
    "ALGORITHM": 4,
}
