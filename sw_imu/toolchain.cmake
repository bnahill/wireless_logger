# Very simple toolchain configuration where the GNU compilers are in your PATH
# and all have a common prefix. Make sure to set TC_PREFIX elsewhere

SET(CMAKE_SYSTEM_NAME  Generic)
    
SET(CMAKE_CROSSCOMPILING 1)
SET(_CMAKE_TOOLCHAIN_PREFIX ${TC_PREFIX})




set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#set(TOOLROOT C:/yagarto-20121222)

#SET(CMAKE_C_COMPILER ${TOOLROOT}/bin/arm-none-eabi-gcc.exe)
#SET(CMAKE_CXX_COMPILER ${TOOLROOT}/bin/arm-none-eabi-g++.exe)
#SET(CMAKE_OBJDUMP ${TOOLROOT}/bin/arm-none-eabi-objdump.exe)
#SET(CMAKE_OBJCOPY ${TOOLROOT}/bin/arm-none-eabi-objcopy.exe)


INCLUDE(CMakeFindBinUtils)

#set(CMAKE_C_COMPILER ${TC_PREFIX}-gcc)
#set(CMAKE_CXX_COMPILER ${TC_PREFIX}-g++)

#set(CMAKE_C_COMPILER /opt/arm-gcc/bin/arm-none-eabi-gcc)

INCLUDE(CMakeDetermineCCompiler)
INCLUDE(CMakeDetermineCXXCompiler)
