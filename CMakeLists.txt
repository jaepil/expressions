cmake_minimum_required(VERSION 3.15)

project(expressions LANGUAGES C CXX)

# Compiler id for Apple Clang is now AppleClang.
# https://www.cmake.org/cmake/help/latest/policy/CMP0025.html
cmake_policy(SET CMP0025 NEW)

# Only interpret if () arguments as variables or keywords when unquoted.
# https://www.cmake.org/cmake/help/latest/policy/CMP0054.html
cmake_policy(SET CMP0054 NEW)

# Support new if () IN_LIST operator.
# https://www.cmake.org/cmake/help/latest/policy/CMP0057.html
cmake_policy(SET CMP0057 NEW)

# Adapted from Apache Kudu: https://github.com/apache/kudu/commit/bd549e13743a51013585
# Honor visibility properties for all target types.
# https://www.cmake.org/cmake/help/latest/policy/CMP0063.html
cmake_policy(SET CMP0063 NEW)

# RPATH settings on macOS do not affect install_name.
# https://cmake.org/cmake/help/latest/policy/CMP0068.html
if (POLICY CMP0068)
    cmake_policy(SET CMP0068 NEW)
endif()

# find_package() uses <PackageName>_ROOT variables.
# https://cmake.org/cmake/help/latest/policy/CMP0074.html
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

# Support Position Independent Executable (PIE) and CheckPIESupported().
if (POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    option(WITH_CCACHE "Build with ccache" ON)
    option(WITH_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
    option(WITH_CLANG_TIDY "Build with clang-tidy" OFF)
else()
    option(WITH_CCACHE "Build with ccache" ON)
    option(WITH_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
    option(WITH_CLANG_TIDY "Build with clang-tidy" OFF)
    option(WITH_DEBUG_SYMBOL "Generate debug symbols for release build" OFF)
endif()
option(WITH_TESTS "Build with unit tests and benchmark tests" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=skylake")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" AND APPLE)
    # Apple M1 is based on ARMv8.5-A instruction set.
    # https://developer.arm.com/documentation/101754/0618/armclang-Reference/armclang-Command-line-Options/-march
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8.5-a+aes+crc+crypto+dotprod+fp+fp16+fp16fml+i8mm+rcpc+simd")
elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64" AND LINUX)
    # AWS Graviton2 is based on ARMv8.2-A instruction set.
    # https://developer.arm.com/documentation/101754/0618/armclang-Reference/armclang-Command-line-Options/-march
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8.2-a+aes+crc+crypto+dotprod+fp+fp16+fp16fml+i8mm+rcpc+simd")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wsign-compare")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=utf-8 -fexec-charset=utf-8")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all")
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize -fstack-protector-strong")
    if (WITH_DEBUG_SYMBOL)
        if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g1")
        endif()
    endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    if (WITH_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-const-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-static-in-inline")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-zero-length-array")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcovered-switch-default")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmove")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes -Wno-strict-aliasing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

if (WITH_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if (CCACHE_FOUND)
        message(STATUS "Found ccache: ${CCACHE_FOUND}")
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_FOUND})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_FOUND})
        # Let ccache preserve C++ comments, because some of them may be
        # meaningful to the compiler
        set(ENV{CCACHE_COMMENTS} "1")
    else()
        find_program(CCACHE_FOUND sccache)
        if (CCACHE_FOUND)
            message(STATUS "Found sccache: ${CCACHE_FOUND}")
            set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_FOUND})
            set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_FOUND})
        endif()
    endif()
endif()

include(CheckPIESupported)
check_pie_supported(OUTPUT_VARIABLE PIE_ERROR LANGUAGES C CXX)
if (CMAKE_CXX_LINK_PIE_SUPPORTED)
    message(STATUS "PIC option is enabled")
    set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
else()
    message(STATUS "PIC option is not supported: <${PIE_ERROR}>")
    set(CMAKE_POSITION_INDEPENDENT_CODE FALSE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)

    if (WITH_SANITIZERS)
        string(CONCAT CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}
               " -fno-omit-frame-pointer"
               " -fsanitize=address -fsanitize-address-use-after-scope"
               " -fsanitize-address-use-after-return=runtime"
               " -fsanitize=undefined"
               " -fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/build-support/asan.supp")
        string(CONCAT CMAKE_EXE_LINKER_FLAGS_DEBUG ${CMAKE_EXE_LINKER_FLAGS_DEBUG}
               " -fno-omit-frame-pointer"
               " -fsanitize=address -fsanitize-address-use-after-scope"
               " -fsanitize-address-use-after-return=runtime"
               " -fsanitize=undefined"
               " -fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/build-support/asan.supp")
    endif()
else()
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)
    if (IPO_SUPPORTED)
        message(STATUS "IPO/LTO options are enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

        if (CMAKE_CXX_COMPILER_ID STREQUAL Clang
            AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 10)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fwhole-program-vtables")
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -fwhole-program")
        endif()
    else()
        message(STATUS "IPO/LTO options are not supported: <${IPO_ERROR}>")
    endif()

    if (WITH_SANITIZERS)
        string(CONCAT CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}
               " -fno-omit-frame-pointer"
               " -fsanitize=address -fsanitize-address-use-after-scope"
               " -fsanitize-address-use-after-return=runtime"
               " -fsanitize=undefined"
               " -fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/build-support/asan.supp")
        string(CONCAT CMAKE_EXE_LINKER_FLAGS_RELEASE ${CMAKE_EXE_LINKER_FLAGS_RELEASE}
               " -fno-omit-frame-pointer"
               " -fsanitize=address -fsanitize-address-use-after-scope"
               " -fsanitize-address-use-after-return=runtime"
               " -fsanitize=undefined"
               " -fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/build-support/asan.supp")
    endif()
endif()

if (WITH_TESTS)
    add_compile_definitions(WITH_TESTS)
endif()

if (WITH_SANITIZERS)
    add_compile_definitions(WITH_SANITIZERS ADDRESS_SANITIZER UNDEFINED_SANITIZER)
endif()

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wnon-virtual-dtor")
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
endif()

# Print configurations
message(STATUS "Build configurations:")
message(STATUS "  CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "  CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "  CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "  CMAKE_EXE_LINKER_FLAGS_DEBUG: ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "  CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "  CMAKE_EXE_LINKER_FLAGS_RELEASE: ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "  CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    message(STATUS "  CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO: ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}")
elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "  CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
    message(STATUS "  CMAKE_EXE_LINKER_FLAGS_MINSIZEREL: ${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}")
endif()
message(STATUS "  CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "  CMAKE_CXX_COMPILER_LAUNCHER: ${CMAKE_CXX_COMPILER_LAUNCHER}")
message(STATUS "  CMAKE_INTERPROCEDURAL_OPTIMIZATION: ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}")
message(STATUS "  CMAKE_POSITION_INDEPENDENT_CODE: ${CMAKE_POSITION_INDEPENDENT_CODE}")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
list(APPEND STD_LIBS Threads::Threads)

find_package(Boost REQUIRED COMPONENTS
    regex system
)
include_directories(${Boost_INCLUDE_DIRS})
add_compile_definitions(
    BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
    BOOST_MPL_LIMIT_LIST_SIZE=50
)
list(APPEND THIRDPARTY_LIBS ${Boost_LIBRARIES})

find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBS fmt)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM-${LLVM_PACKAGE_VERSION}: ${LLVM_INSTALL_PREFIX}")
include_directories(${LLVM_INCLUDE_DIR})
if (LINUX)
    list(APPEND THIRDPARTY_LIBS
        LLVM-${LLVM_VERSION_MAJOR}
        clang-${LLVM_VERSION_MAJOR}
        clang-cpp${LLVM_VERSION_MAJOR}
    )
else()
    list(APPEND THIRDPARTY_LIBS LLVM clang clang-cpp)
endif()

find_package(GTest CONFIG REQUIRED)
include_directories(${GTest_INCLUDE_DIRS})
list(APPEND THIRDPARTY_LIBS gtest)

find_package(benchmark REQUIRED)
list(APPEND THIRDPARTY_LIBS benchmark)

# Add bundled modules
if (APPLE)
    add_compile_definitions(BACKWARD_HAS_LIBUNWIND=1)
    list(APPEND STD_LIBS unwind)
elseif (LINUX)
    add_compile_definitions(BACKWARD_HAS_DW=1)
    list(APPEND STD_LIBS dw)
endif()
include_directories(deps)
include_directories(deps/include)
add_subdirectory(deps)

# Source files
include_directories(src)
add_subdirectory(src)

if (WITH_TESTS)
    include_directories(tests)
    file(GLOB_RECURSE GTEST_SOURCE_FILES CONFIGURE_DEPENDS tests/*.cpp)
    list(SORT GTEST_SOURCE_FILES COMPARE STRING CASE SENSITIVE ORDER ASCENDING)

    add_library(expressions-tests OBJECT ${GTEST_SOURCE_FILES})
endif()

if (LINUX)
    list(APPEND STD_LIBS rt)
endif()

add_executable(
    expressions
    src/expressions/main.cpp
)
target_link_libraries(
    expressions PRIVATE
    expressions-src
    expressions-deps
)
if (WITH_TESTS)
    target_link_libraries(
        expressions PRIVATE expressions-tests
    )
endif()
target_link_libraries(
    expressions PRIVATE

    ${STD_LIBS}
    ${THIRDPARTY_LIBS}
)
