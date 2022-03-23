cmake_minimum_required(VERSION 3.17)

# Name of the CMake project
project(Deus)

# C++ standard you want to use for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/import)

option(UNIT_TESTS "When sets to ON, build the unit tests" OFF)

include(FetchYamlCPP)

if(UNIT_TESTS)
    # Bin name of the unit tests
    set(BIN unit_tests)
else()
    # Bin name of the project
    set(BIN deus)
endif()

# Fetch GoogleTest
if(UNIT_TESTS)
    find_package(googletest QUIET)
    include(FetchGoogleTest)
endif()

# Build flags
if(UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
elseif(WIN32)
    add_compile_options(/W4)
endif()

# Location of the main source file
set(MAIN src/main.cpp)

# List of source files
set(SOURCES
    src/main.cpp
    src/config.cpp
    src/resource.cpp
)

# List of tests source files
set(TEST_SOURCES
    tests/test.cpp
)

# Executable settings
if(UNIT_TESTS)
    add_executable(${BIN}
        ${SOURCES}
        ${TEST_SOURCES}
    )
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(${BIN})
    target_link_libraries(${BIN} PRIVATE gtest_main)
else()
    add_executable(${BIN}
        ${MAIN}
        ${SOURCES}
    )
endif()

# Include path of the project
target_include_directories(${BIN} PRIVATE include)

target_link_libraries(${BIN} PUBLIC yaml-cpp)