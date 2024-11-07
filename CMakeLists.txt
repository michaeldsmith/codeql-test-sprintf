cmake_minimum_required(VERSION 3.10.0)

project (CODE_QL_TEST_SPRINTF DESCRIPTION "A test of CodeQL" LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For GCC version 8, link against stdc++fs for filesystem support
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    link_libraries(stdc++fs)
endif()

if (MSVC)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /D \"_CRT_SECURE_NO_WARNINGS\"")
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fexceptions -Wall -Wextra -Wconversion -Wunused-parameter -Wsign-conversion -Wdeprecated-declarations")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set( CODE_QL_TEST_SPRINTF "codeql-test-sprintf")
add_executable(${CODE_QL_TEST_SPRINTF} source/main.cpp)

INSTALL(TARGETS ${CODE_QL_TEST_SPRINTF}
        DESTINATION bin)

# add address sanitizer build type
set(CMAKE_CXX_FLAGS_ASAN
    "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
    CACHE STRING "Flags used by the C++ compiler during AddressSanitizer builds."
    FORCE)

if( NOT DEFINED CMAKE_BUILD_TYPE )
  MESSAGE( STATUS "To use AddressSanitizer, use \"cmake .. -DCMAKE_BUILD_TYPE=asan\"" )
elseif( CMAKE_BUILD_TYPE STREQUAL "asan" )
  MESSAGE( STATUS "Using AddressSanitizer - CMAKE_BUILD_TYPE : \"${CMAKE_BUILD_TYPE}\"")
else()
  MESSAGE( STATUS "CMAKE_BUILD_TYPE : \"${CMAKE_BUILD_TYPE}\"")
  MESSAGE( STATUS "To use AddressSanitizer, use \"cmake .. -DCMAKE_BUILD_TYPE=asan\"" )
endif()

# add basic tests
enable_testing()

add_test(NAME "one-arg" COMMAND ${CODE_QL_TEST_SPRINTF} testname)
add_test(NAME "three-args" COMMAND ${CODE_QL_TEST_SPRINTF} testname 1 10)
add_test(NAME "three-args-printf-style" COMMAND ${CODE_QL_TEST_SPRINTF} input.%07d.tif 1 10)
