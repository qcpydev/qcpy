import os
from .connect import Connect

FILE_PATH = os.path.dirname(os.path.realpath(__file__))

BUILD_PATH = "/core/build/"
CONNECT_NAME = "libqcpy_connect.so"
CORE_NAME = "qcpy_core"

QCPY_CONNECT_FILE_PATH = (
    str(os.path.abspath(os.path.join(FILE_PATH, os.pardir))) + BUILD_PATH + CONNECT_NAME
)

QCPY_CORE_FILE_PATH = (
    str(os.path.abspath(os.path.join(FILE_PATH, os.pardir))) + BUILD_PATH + CORE_NAME
)
# TODO: set up arguments, check if custom was imported from qcpy, and then dont boot until we get the function call
qcpy_connect = Connect([], QCPY_CONNECT_FILE_PATH, QCPY_CORE_FILE_PATH)
