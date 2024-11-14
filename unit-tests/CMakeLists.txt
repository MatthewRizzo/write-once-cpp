cmake_minimum_required(VERSION 3.2)
project(write_once_cpp_tests C CXX)

# Mainline gtest requires C++14 features!
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")
add_subdirectory(extern/googletest)


include(CTest)

enable_testing()

add_executable(write_once_cpp_test
    test_main.cpp
    write_once_test.cpp
)

target_link_libraries(write_once_cpp_test
    write_once
    gtest
)
target_include_directories(write_once_cpp_test PUBLIC
    extern/googletest/googletest/include
)
set_target_properties(write_once_cpp_test PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS write_once_cpp_test DESTINATION bin)

add_test(
    NAME test_write_once
    COMMAND ${PROJECT_INSTALL_BIN_DIR}/write_once_cpp_test
    WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR}
)
