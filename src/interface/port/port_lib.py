import ctypes
import os
from enum import IntEnum

FILE_PATH = os.path.dirname(os.path.realpath(__file__))
PORT_BIN_FILE_PATH = str(FILE_PATH) + "port.so"
port_cross = ctypes.CDLL(PORT_BIN_FILE_PATH)
