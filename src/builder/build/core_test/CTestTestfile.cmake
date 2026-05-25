# CMake generated Testfile for 
# Source directory: /home/brennanf/Projects/qcpy_dev/qcpy/src/test/core_test
# Build directory: /home/brennanf/Projects/qcpy_dev/qcpy/src/builder/build/core_test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[run_tests]=] "/home/brennanf/Projects/qcpy_dev/qcpy/src/builder/build/bin/run_tests")
set_tests_properties([=[run_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/brennanf/Projects/qcpy_dev/qcpy/src/test/core_test/CMakeLists.txt;10;add_test;/home/brennanf/Projects/qcpy_dev/qcpy/src/test/core_test/CMakeLists.txt;0;")
subdirs("test_qlog")
