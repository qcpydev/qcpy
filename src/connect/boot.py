import os
from .connect import Connect

FILE_PATH = os.path.dirname(os.path.realpath(__file__))
BUILD_PATH = "/builder/build/"

CONNECT_NAME = "lib/libqcpy_connect.so"
CORE_NAME = "bin/qcpy_core"
QUACK_NAME = "bin/quack_core"
QUACK_GPU_NAME = "bin/quack_core_gpu"

QCPY_CONNECT_FILE_PATH = (
    str(os.path.abspath(os.path.join(FILE_PATH, os.pardir))) + BUILD_PATH + CONNECT_NAME
)

QCPY_CORE_FILE_PATH = (
    str(os.path.abspath(os.path.join(FILE_PATH, os.pardir))) + BUILD_PATH + CORE_NAME
)

QUACK_FILE_PATH = (
    str(os.path.abspath(os.path.join(FILE_PATH, os.pardir))) + BUILD_PATH + QUACK_NAME
)


QUACK_GPU_FILE_PATH = (
    str(os.path.abspath(os.path.join(FILE_PATH, os.pardir))) + BUILD_PATH + QUACK_GPU_NAME
)

# TODO: set up arguments, check if custom was imported from qcpy, and then dont boot until we get the function call
qcpy_connect = Connect([], QCPY_CONNECT_FILE_PATH, QCPY_CORE_FILE_PATH, QUACK_FILE_PATH, QUACK_GPU_FILE_PATH)
