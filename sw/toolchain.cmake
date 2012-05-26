# Very simple toolchain configuration where the GNU compilers are in your PATH
# and all have a common prefix. Make sure to set TC_PREFIX elsewhere

SET(CMAKE_SYSTEM_NAME  Generic)

SET(_CMAKE_TOOLCHAIN_PREFIX ${TC_PREFIX})

INCLUDE(CMakeFindBinUtils)
INCLUDE(CMakeDetermineCCompiler)
INCLUDE(CMakeDetermineCXXCompiler)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
