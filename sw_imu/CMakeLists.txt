# CMakeLists for STM32F4 using the Sourcery toolchain and OpenOCD with both
# C and C++ sources
# 
# To adapt this for any other STM32 chip should be trivial
# To adapt to any other ARM controller shouldn't be much worse
#
# A few convenience targets are provided for OpenOCD, GDB, and Ctags
#
# The structure is as follows
#  /CMSIS
#  /STM32F4xx_StdPeriph...
#  /src (application code)
#  /src/system (application code - low-level configuration)
#  /stm32_flash.ld (linker script for !!!STM32F4 only!!! from Atollic TrueStudio)
#
# Author: Ben Nahill <bnahill@gmail.com>
#



# Use the GNU ARM EABI toolchain
set(TC_PREFIX arm-none-eabi-)

# Include the correct toolchain file and set a variable so that we can
# determine the correct host type later
if(WIN32)
	message("Detected WIN32 build environment")
	set(IS_UNIX false)
	set(DETECTED_HOST "Windows")
	set(CMAKE_TOOLCHAIN_FILE toolchain_win32.cmake)
	set(ENV{HOME} ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32)

if(UNIX)
	message("Detected UNIX build environment")
	set(DETECTED_HOST "UNIX")
	set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)
endif(UNIX)

set(BUILD_HOST ${DETECTED_HOST} CACHE STRING "Build host (Check this!)")


set(TARGET imu_logger)
set(OUTFILE ${TARGET}.elf)

project(${TARGET})
cmake_minimum_required(VERSION 2.8)


########################################################
# Source locations
########################################################

set(IMU_DIR imu)
# Path to CMSIS
set(CMSIS_DIR CMSIS)
set(OS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/os)











########################################################
# Build Options
########################################################

set(OPTIMIZATION "-O0")
set(LTO )#"-flto")

set(MCU cortex-m4)

set(MCFLAGS "-mcpu=${MCU} -g -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -D__NEWLIB__ -fomit-frame-pointer")

add_definitions(${MCFLAGS})
add_definitions(-DHSE_VALUE=8000000 -DARM_MATH_CM4 -D__FPU_PRESENT)

set(LINKER_SCRIPT ${OS_DIR}/ports/GCC/ARMCMx/STM32F4xx/ld/STM32F405xG.ld)

###################################################
# Compiler Flags
###################################################
set(CXX_WARNINGS "-Waddress -Warray-bounds -Wc++11-compat -Wchar-subscripts ")
set(CXX_WARNINGS "${CXX_WARNINGS} -Wcomment -Wformat -Wmaybe-uninitialized -Wnonnull -Wparentheses -Wreturn-type ")
set(CXX_WARNINGS "${CXX_WARNINGS} -Wsequence-point -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs ")
set(CXX_WARNINGS "${CXX_WARNINGS} -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var")

set(C_WARNINGS "-Wall")

# Flags for both C and C++ compilers
set(COMMON_FLAGS "${LTO} ${OPTIMIZATION} -ffunction-sections -fdata-sections ${MCFLAGS}")
# Eliminate the need for a bunch of extra support code that may not work on the chip anyways
set(CMAKE_CXX_FLAGS "--std=gnu++11 -fno-rtti -fno-exceptions ${CXX_WARNINGS} ${COMMON_FLAGS}")
# Use the new C11 standard
set(CMAKE_C_FLAGS "--std=gnu1x ${C_WARNINGS} ${COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${LTO} ${OPTIMIZATION} -Wl,-Map=${TARGET}.map,--cref,-static,--gc-sections -nostartfiles  ${MCFLAGS} ${CMAKE_EXE_LINKER_FLAGS}")


include_directories(
	.
	src
	${OS_DIR}/kernel/include
	${OS_DIR}/hal/include
	${OS_DIR}/hal/platforms/STM32F4xx
	${OS_DIR}/ports/GCC/ARMCMx
	${OS_DIR}/ports/GCC/ARMCMx/STM32F4xx
	${OS_DIR}/ports/common/ARMCMx
	${OS_DIR}/ports/common/ARMCMx/CMSIS/include
	${OS_DIR}/hal/platforms/STM32
	${OS_DIR}/hal/platforms/STM32F4xx
	${OS_DIR}/hal/platforms/STM32/GPIOv2
	${OS_DIR}/hal/platforms/STM32/I2Cv1
	${OS_DIR}/hal/platforms/STM32/SPIv1
	${OS_DIR}/hal/platforms/STM32/OTGv1
	
	
	${IMU_DIR}
	
	src/platform
)


########################################################
# Sources
########################################################


set(OS_SOURCES
	${OS_DIR}/ports/GCC/ARMCMx/crt0.c
	${OS_DIR}/ports/GCC/ARMCMx/STM32F4xx/vectors.c
	${OS_DIR}/ports/GCC/ARMCMx/chcore.c
	${OS_DIR}/ports/GCC/ARMCMx/chcore_v7m.c
	${OS_DIR}/ports/common/ARMCMx/nvic.c
	${OS_DIR}/kernel/src/chsys.c
	${OS_DIR}/kernel/src/chdebug.c
	${OS_DIR}/kernel/src/chlists.c
	${OS_DIR}/kernel/src/chvt.c
	${OS_DIR}/kernel/src/chschd.c
	${OS_DIR}/kernel/src/chthreads.c
	${OS_DIR}/kernel/src/chdynamic.c
	${OS_DIR}/kernel/src/chregistry.c
	${OS_DIR}/kernel/src/chsem.c
	${OS_DIR}/kernel/src/chmtx.c
	${OS_DIR}/kernel/src/chcond.c
	${OS_DIR}/kernel/src/chevents.c
	${OS_DIR}/kernel/src/chmsg.c
	${OS_DIR}/kernel/src/chmboxes.c
	${OS_DIR}/kernel/src/chqueues.c
	${OS_DIR}/kernel/src/chmemcore.c
	${OS_DIR}/kernel/src/chheap.c
	${OS_DIR}/kernel/src/chmempools.c
	${OS_DIR}/hal/src/hal.c
	${OS_DIR}/hal/src/adc.c
	${OS_DIR}/hal/src/can.c
	${OS_DIR}/hal/src/ext.c
	${OS_DIR}/hal/src/gpt.c
	${OS_DIR}/hal/src/i2c.c
	${OS_DIR}/hal/src/icu.c
	${OS_DIR}/hal/src/mac.c
	${OS_DIR}/hal/src/mmc_spi.c
	${OS_DIR}/hal/src/pal.c
	${OS_DIR}/hal/src/pwm.c
	${OS_DIR}/hal/src/rtc.c
	${OS_DIR}/hal/src/sdc.c
	${OS_DIR}/hal/src/serial.c
	${OS_DIR}/hal/src/serial_usb.c
	${OS_DIR}/hal/src/spi.c
	${OS_DIR}/hal/src/tm.c
	${OS_DIR}/hal/src/uart.c
	${OS_DIR}/hal/src/usb.c
	${OS_DIR}/hal/platforms/STM32F4xx/adc_lld.c
	${OS_DIR}/hal/platforms/STM32F4xx/hal_lld.c
	${OS_DIR}/hal/platforms/STM32F4xx/stm32_dma.c
	${OS_DIR}/hal/platforms/STM32F4xx/ext_lld_isr.c
	${OS_DIR}/hal/platforms/STM32/GPIOv2/pal_lld.c
	${OS_DIR}/hal/platforms/STM32/I2Cv1/i2c_lld.c
	${OS_DIR}/hal/platforms/STM32/SPIv1/spi_lld.c
	${OS_DIR}/hal/platforms/STM32/OTGv1/usb_lld.c
	${OS_DIR}/hal/platforms/STM32/gpt_lld.c
	${OS_DIR}/hal/platforms/STM32/pwm_lld.c
	${OS_DIR}/hal/platforms/STM32/i2s_lld.c
	${OS_DIR}/hal/platforms/STM32/sdc_lld.c
	${OS_DIR}/hal/platforms/STM32/ext_lld.c
)

set(APP_SOURCES
	src/main.cpp
	src/acquisition.cpp
	src/platform/board.c
	src/platform/platform.cpp
	src/ui/ui.cpp
	src/ui/menu.cpp
	src/ui/mainmenu.cpp
	src/ui/sensor_view.cpp
)


set(IMU_SOURCES
	${IMU_DIR}/button.cpp
	${IMU_DIR}/ltc3559.cpp
	${IMU_DIR}/mma8452q.cpp
	${IMU_DIR}/mag3110.cpp
	${IMU_DIR}/i2c.cpp
	${IMU_DIR}/ssd1306.cpp
	${IMU_DIR}/imu.cpp
	
	${IMU_DIR}/graphics/smallfont.cpp
	${IMU_DIR}/graphics/mediumfont.cpp
	${IMU_DIR}/graphics/comicsans4.cpp
	${IMU_DIR}/graphics/courier3.cpp
)


##############################################################
# Executable and library targets
##############################################################

#add_library(imu_lib STATIC ${IMU_SOURCES})
add_executable(${OUTFILE} ${APP_SOURCES} ${OS_SOURCES} ${IMU_SOURCES})
target_link_libraries(${OUTFILE} gcc c)


# Warnings only for application code -- Optimizations only on ST/ARM code
set_target_properties(${OUTFILE} PROPERTIES COMPILE_FLAGS "-W ${OPTIMIZATION}")
#set_target_properties(imu_lib PROPERTIES COMPILE_FLAGS "-W ${OPTIMIZATION}")
#set_target_properties(cmsis_dsp PROPERTIES COMPILE_FLAGS "-w -O2")

##############################################################
# Configuration for GDB
##############################################################

set(GDB    ${TC_PREFIX}gdb)
set(GDBTUI ${TC_PREFIX}gdbtui)

configure_file(${CMAKE_SOURCE_DIR}/start.gdb.in ${CMAKE_CURRENT_BINARY_DIR}/start.gdb)
set(GDB_ARGS -x start.gdb)
if(${DETECTED_HOST} STREQUAL "Windows")
endif(${DETECTED_HOST} STREQUAL "Windows")

if(${DETECTED_HOST} STREQUAL "Linux")
#configure_file(${CMAKE_SOURCE_DIR}/start.gdb.in ${CMAKE_CURRENT_BINARY_DIR}/.gdbinit)
endif(${DETECTED_HOST} STREQUAL "Linux")

add_custom_target(gdb COMMAND ${GDB} ${GDB_ARGS})
add_custom_target(gdbtui COMMAND ${GDBTUI} ${GDB_ARGS})
add_custom_target(ddd COMMAND ddd --gdb --debugger \"${GDB} ${GDB_ARGS}\" > /dev/null 2>&1 &
	COMMENT "Launching DDD"
)

add_dependencies(gdb gdb_script)
add_dependencies(gdbtui gdb_script)
add_dependencies(ddd gdb_script)



##############################################################
# Assembly output
##############################################################

add_custom_command(OUTPUT ${TARGET}.lst COMMAND ${CMAKE_OBJDUMP} -S --disassemble ${OUTFILE} > ${TARGET}.lst DEPENDS ${OUTFILE})
add_custom_target(assembly_output ALL DEPENDS ${TARGET}.lst)

##############################################################
# Configuration for OpenOCD
##############################################################

set(OPENOCD openocd)
set(OPENOCD_SCRIPTS
	board/stm32f4discovery.cfg
)
foreach(s ${OPENOCD_SCRIPTS})
	set(OPENOCD_ARGS ${OPENOCD_ARGS} -f ${s})
endforeach(s)

add_custom_target(openocd COMMAND ${OPENOCD} ${OPENOCD_ARGS} -c "stm32f4x.cpu configure -rtos auto"
	COMMENT "Launching OpenOCD..."
)

add_custom_target(openocd_no_os COMMAND ${OPENOCD} ${OPENOCD_ARGS}
	COMMENT "Launching OpenOCD..."
)

##############################################################
# Doxygen Output
##############################################################

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)

#############################################################
# HEX file generation
#############################################################

add_custom_target(${TARGET}.hex ALL
	COMMAND ${TC_PREFIX}objcopy -O ihex ${OUTFILE} ${TARGET}.hex
	DEPENDS ${OUTFILE}
	COMMENT "Generating Intel HEX output..."
)
