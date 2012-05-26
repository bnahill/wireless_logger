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


# Use the Sourcery ARM EABI toolchain
set(TC_PREFIX arm-none-eabi-)
set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

set(TARGET imu_logger)
set(OUTFILE ${TARGET}.elf)

project(${TARGET})
cmake_minimum_required(VERSION 2.8)

# Path to the STM32F4 Peripheral Library driver directory
set(STD_PERIPH_DIR STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver)
# Path to the ST USB device driver
set(ST_USB_DEV_DIR usb_device)
# Path to the ST USB device driver
set(ST_USB_OTG_DIR usb_otg)
# Path to the USB application driver
set(ST_USB_APP_DIR usb_cdc)
# Path to CMSIS
set(CMSIS_DIR STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS)
# Path to IMU library drivers
set(MCU cortex-m3)


set(MCFLAGS "-mcpu=${MCU} -mthumb")

add_definitions(${MCFLAGS})
add_definitions(-g -ffunction-sections  -fdata-sections -fpromote-loop-indices -Wredundant-decls -Wshadow -Wcast-qual -Wcast-align)
add_definitions(-DUSE_STDPERIPH_DRIVER -DSTM32F10X_HD -DHSE_VALUE=8000000 -DARM_MATH_CM3)

# Eliminate the need for a bunch of extra support code that may not work on the chip anyways
set(CMAKE_CXX_FLAGS "--std=c++0x -fno-rtti -fno-exceptions")
# Use the new C11 standard
set(CMAKE_C_FLAGS "--std=c1x")
set(CMAKE_EXE_LINKER_FLAGS "-Tlink.ld -flto  -Wl,-Map=${TARGET}.map,--cref,--gc-sections -nostdlib ${MCFLAGS} ${CMAKE_EXE_LINKER_FLAGS}")

set(STD_PERIPH_SRC_DIR ${STD_PERIPH_DIR}/src)
set(APP_DIR src)
set(FATFS_DIR fatfs/src)

set(STM_SOURCES
	${STD_PERIPH_SRC_DIR}/misc.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_dac.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_gpio.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_sdio.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_adc.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_dbgmcu.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_i2c.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_spi.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_bkp.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_dma.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_iwdg.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_tim.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_can.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_exti.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_pwr.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_usart.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_cec.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_flash.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_rcc.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_wwdg.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_crc.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_fsmc.c
	${STD_PERIPH_SRC_DIR}/stm32f10x_rtc.c
)

set(APP_SOURCES
	${APP_DIR}/button.c
	${APP_DIR}/logger.c
	${APP_DIR}/lsm303.c
	${APP_DIR}/sdio.c
	${APP_DIR}/system_stm32f10x.c
	${APP_DIR}/fs_hal.c
	${APP_DIR}/lpry.c
	${APP_DIR}/main.c
	${APP_DIR}/sdio_ll.c
	${APP_DIR}/i2c.c
	${APP_DIR}/lps001.c
	${APP_DIR}/rtc.c
	${APP_DIR}/stm32f10x_it.c
	${APP_DIR}/startup_stm32f10x_hd.s
)

set(FATFS_SOURCES
	${FATFS_DIR}/ff.c
)

# Please don't just ignore that assembly file...
set_property(SOURCE ${APP_DIR}/startup_stm32f10x_hd.s PROPERTY LANGUAGE C)

##############################################################
# Options
##############################################################


##############################################################
# Executable and library targets
##############################################################

include_directories(
	.
	src
	${CMSIS_DIR}/CM3/CoreSupport
	${CMSIS_DIR}/CM3/DeviceSupport/ST/STM32F10x
	${STD_PERIPH_DIR}/inc
	${FATFS_DIR}
)

add_library(std_periph STATIC ${STM_SOURCES})
set(LIBS ${LIBS} std_periph)

add_library(fatfs STATIC ${FATFS_SOURCES})
set(LIBS ${LIBS} fatfs)

add_executable(${OUTFILE} ${APP_SOURCES})
target_link_libraries(${OUTFILE} ${LIBS})

# Funky workaround to make target dependent on linker script
add_custom_command(OUTPUT link.ld COMMAND cat ${CMAKE_SOURCE_DIR}/stm32_flash.ld > link.ld DEPENDS ${CMAKE_SOURCE_DIR}/stm32_flash.ld)
add_custom_target(linker_script DEPENDS link.ld)
add_dependencies(${OUTFILE} linker_script)

# Warnings only for application code -- Optimizations only on ST/ARM code
set_target_properties(${OUTFILE} PROPERTIES COMPILE_FLAGS "-Wall -Wclobbered -Wempty-body -Wignored-qualifiers -Woverride-init -Wsign-compare -Wtype-limits -Wuninitialized -O0")
set_target_properties(std_periph PROPERTIES COMPILE_FLAGS "-Wall -O0")

##############################################################
# Ctags
##############################################################

set(CTAG_OPTS -R --c-kinds=+p --fields=+iaS --extra=+q)
set(CTAG_SEARCH_LIST
	${STD_PERIPH_DIR}/inc
	${CMSIS_DIR}/CM3/CoreSupport
	${CMSIS_DIR}/CM3/DeviceSupport/ST/STM32F10x
)

foreach(p ${CTAG_SEARCH_LIST})
	set(CTAG_SEARCH ${CTAG_SEARCH} ${CMAKE_SOURCE_DIR}/${p})
endforeach(p)

add_custom_target(ctags
	ctags ${CTAG_OPTS} -f ${CMAKE_SOURCE_DIR}/src/ext_tags ${CTAG_SEARCH}
	COMMENT "Generating Ctags..."
)

##############################################################
# Configuration for GDB
##############################################################

set(GDB    ${TC_PREFIX}gdb)
set(GDBTUI ${TC_PREFIX}gdbtui)

set(GDB_ARGS -ex "target remote localhost:3333" -ex "mon reset init")
add_custom_target(gdb COMMAND ${GDB} ${GDB_ARGS} ${OUTFILE})
add_custom_target(gdbtui COMMAND ${GDBTUI} ${GDB_ARGS} ${OUTFILE})
add_custom_target(ddd COMMAND ddd --gdb --debugger \"${GDB} ${GDB_ARGS}\" ${OUTFILE} > /dev/null 2>&1 &
	COMMENT "Launching DDD"
)

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

add_custom_target(openocd COMMAND ${OPENOCD} ${OPENOCD_ARGS}
	COMMENT "Launching OpenOCD..."
)

##############################################################
# Doxygen Output
##############################################################

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
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
