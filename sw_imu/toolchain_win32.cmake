SET(CMAKE_ECLIPSE_VERSION "4.2 (Juno)")
SET(CMAKE_C_COMPILER arm-none-eabi-gcc.exe)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++.exe)
SET(CMAKE_OBJDUMP arm-none-eabi-objdump.exe)
SET(CMAKE_OBJCOPY arm-none-eabi-objcopy.exe)

# Very simple toolchain configuration where the GNU compilers are in your PATH
# and all have a common prefix. Make sure to set TC_PREFIX elsewhere

SET(CMAKE_SYSTEM_NAME  Generic)
    
SET(CMAKE_CROSSCOMPILING 1)
SET(_CMAKE_TOOLCHAIN_PREFIX ${TC_PREFIX})


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

INCLUDE(CMakeFindBinUtils)
INCLUDE(CMakeDetermineCCompiler)
INCLUDE(CMakeDetermineCXXCompiler)
