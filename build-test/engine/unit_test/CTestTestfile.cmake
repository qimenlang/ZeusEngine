# CMake generated Testfile for 
# Source directory: D:/repos/engines/ZeusEngine/engine/unit_test
# Build directory: D:/repos/engines/ZeusEngine/build-test/engine/unit_test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(UnitTest "D:/repos/engines/ZeusEngine/build-test/engine/unit_test/Debug/UnitTest.exe" "-s" "--verbosity" "high")
  set_tests_properties(UnitTest PROPERTIES  _BACKTRACE_TRIPLES "D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;14;add_test;D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(UnitTest "D:/repos/engines/ZeusEngine/build-test/engine/unit_test/Release/UnitTest.exe" "-s" "--verbosity" "high")
  set_tests_properties(UnitTest PROPERTIES  _BACKTRACE_TRIPLES "D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;14;add_test;D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(UnitTest "D:/repos/engines/ZeusEngine/build-test/engine/unit_test/MinSizeRel/UnitTest.exe" "-s" "--verbosity" "high")
  set_tests_properties(UnitTest PROPERTIES  _BACKTRACE_TRIPLES "D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;14;add_test;D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(UnitTest "D:/repos/engines/ZeusEngine/build-test/engine/unit_test/RelWithDebInfo/UnitTest.exe" "-s" "--verbosity" "high")
  set_tests_properties(UnitTest PROPERTIES  _BACKTRACE_TRIPLES "D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;14;add_test;D:/repos/engines/ZeusEngine/engine/unit_test/CMakeLists.txt;0;")
else()
  add_test(UnitTest NOT_AVAILABLE)
endif()
