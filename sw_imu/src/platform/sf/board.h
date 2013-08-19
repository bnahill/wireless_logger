/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

//! @addtogroup Platform
//! @{
//! @addtogroup ChibiOS
//! @{

/*
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F4_DISCOVERY
#define BOARD_NAME              "ST STM32F4-Discovery"

/*
 * Board frequencies.
 * NOTE: The LSE crystal is not fitted by default on the board.
 */
#define STM32_LSECLK            32768
//#define STM32_HSECLK            8000000

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD               300

/*
 * MCU type as defined in the ST header file stm32f4xx.h.
 */
#define STM32F4XX

/*
 * IO pins assignments.
 */

#define GPIOA_UART4_TX          0
#define GPIOA_UART4_RX          1
#define GPIOA_OLED_NDC          3
#define GPIOA_SPI1_SCK          5
#define GPIOA_SPI1_MISO         6
#define GPIOA_SPI1_MOSI         7
#define GPIOA_I2C3_SCL          8
#define GPIOA_VBUS_FS           9
#define GPIOA_OTG_FS_ID         10
#define GPIOA_OTG_FS_DM         11
#define GPIOA_OTG_FS_DP         12
#define GPIOA_SWDIO             13
#define GPIOA_SWCLK             14

#define GPIOB_SWO               3
#define GPIOB_GYRO_NCS          5
#define GPIOB_PRS_NCS           6
#define GPIOB_FLASH_NCS         7
#define GPIOB_I2C1_SCL          8
#define GPIOB_I2C1_SDA          9
#define GPIOB_SPI2_NSS          12
#define GPIOB_SPI2_SCK          13
#define GPIOB_SPI2_MISO         14
#define GPIOB_SPI2_MOSI         15

#define GPIOC_GYR_INT           1
#define GPIOC_BATT_MON          4
#define GPIOC_BUTTON1           5
#define GPIOC_REG_MODE          6
#define GPIOC_REG_NCHG          7
#define GPIOC_REG_NUSB_CONN     8
#define GPIOC_I2C3_SDA          9
#define GPIOC_SPI3_SCK          10
#define GPIOC_SPI3_MISO         11
#define GPIOC_SPI3_MOSI         12

#define GPIOD_UART2_TX          5
#define GPIOD_UART2_RX          6

#define GPIOE_ACC_INT1          0
#define GPIOE_LED3              2
#define GPIOE_LED4              3
#define GPIOE_OLED_EN           4
#define GPIOE_LED1              5
#define GPIOE_BUTTON4           8
#define GPIOE_BT_NRESET         12
#define GPIOE_CC1101_NCS        15

#define GPIOF_BUTTON2           6
#define GPIOF_BUTTON3           7
#define GPIOF_OLED_NCS          10

#define GPIOH_OSC_IN            0
#define GPIOH_OSC_OUT           1
#define GPIOH_OLED_NRES         2
#define GPIOH_I2C2_SCL          4
#define GPIOH_I2C2_SDA          5

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUDR_FLOATING(n)        (0U << ((n) * 2))
#define PIN_PUDR_PULLUP(n)          (1U << ((n) * 2))
#define PIN_PUDR_PULLDOWN(n)        (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))
#define PIN(n)                      (1U << (n))
/*
 * Port A setup.
 * All input with pull-up except:
 * PA0  - GPIOA_BUTTON          (input floating).
 * PA4  - GPIOA_LRCK            (alternate 6).
 * PA5  - GPIOA_SPC             (alternate 5).
 * PA6  - GPIOA_SDO             (alternate 5).
 * PA7  - GPIOA_SDI             (alternate 5).
 * PA9  - GPIOA_VBUS_FS         (input floating).
 * PA10 - GPIOA_OTG_FS_ID       (alternate 10).
 * PA11 - GPIOA_OTG_FS_DM       (alternate 10).
 * PA12 - GPIOA_OTG_FS_DP       (alternate 10).
 * PA13 - GPIOA_SWDIO           (alternate 0).
 * PA14 - GPIOA_SWCLK           (alternate 0).
 */
#define VAL_GPIOA_MODER     (PIN_MODE_ALTERNATE(GPIOA_UART4_TX) |           \
                             PIN_MODE_ALTERNATE(GPIOA_UART4_RX) |           \
                             PIN_MODE_INPUT(2) |                            \
                             PIN_MODE_OUTPUT(GPIOA_OLED_NDC) |              \
                             PIN_MODE_INPUT(4) |                            \
                             PIN_MODE_ALTERNATE(GPIOA_SPI1_SCK) |           \
                             PIN_MODE_ALTERNATE(GPIOA_SPI1_MISO) |          \
                             PIN_MODE_ALTERNATE(GPIOA_SPI1_MOSI) |          \
                             PIN_MODE_ALTERNATE(GPIOA_I2C3_SCL) |           \
                             PIN_MODE_INPUT(GPIOA_VBUS_FS) |                \
                             PIN_MODE_ALTERNATE(GPIOA_OTG_FS_ID) |          \
                             PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |          \
                             PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |          \
                             PIN_MODE_ALTERNATE(GPIOA_SWDIO) |              \
                             PIN_MODE_ALTERNATE(GPIOA_SWCLK))
#define VAL_GPIOA_OTYPER    0x00000000
#define VAL_GPIOA_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOA_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLDOWN(GPIOA_VBUS_FS) |             \
                             PIN_PUDR_FLOATING(GPIOA_OTG_FS_ID) |           \
                             PIN_PUDR_FLOATING(GPIOA_OTG_FS_DM) |           \
                             PIN_PUDR_FLOATING(GPIOA_OTG_FS_DP) |           \
                             PIN_PUDR_PULLUP(GPIOA_SWDIO) |                 \
                             PIN_PUDR_PULLDOWN(GPIOA_SWCLK) |               \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOA_ODR       (0xFFFFFFFF &                                   \
                           ~(0))
#define VAL_GPIOA_AFRL      (PIN_AFIO_AF(GPIOA_SPI1_SCK, 5) |               \
                             PIN_AFIO_AF(GPIOA_SPI1_MISO, 5) |              \
                             PIN_AFIO_AF(GPIOA_SPI1_MOSI, 5))
#define VAL_GPIOA_AFRH      (PIN_AFIO_AF(GPIOA_I2C3_SCL, 4) |               \
                             PIN_AFIO_AF(GPIOA_OTG_FS_ID, 10) |             \
                             PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10) |             \
                             PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10) |             \
                             PIN_AFIO_AF(GPIOA_SWDIO, 0) |                  \
                             PIN_AFIO_AF(GPIOA_SWCLK, 0))

/*
 * Port B setup.
 */
#define VAL_GPIOB_MODER     (PIN_MODE_INPUT(0) |                            \
                             PIN_MODE_INPUT(1) |                            \
                             PIN_MODE_INPUT(2) |                            \
                             PIN_MODE_ALTERNATE(GPIOB_SWO) |                \
                             PIN_MODE_INPUT(4) |                            \
                             PIN_MODE_OUTPUT(GPIOB_GYRO_NCS) |              \
                             PIN_MODE_OUTPUT(GPIOB_PRS_NCS) |               \
                             PIN_MODE_OUTPUT(GPIOB_FLASH_NCS) |             \
                             PIN_MODE_ALTERNATE(GPIOB_I2C1_SCL) |           \
                             PIN_MODE_ALTERNATE(GPIOB_I2C1_SDA) |           \
                             PIN_MODE_INPUT(10) |                           \
                             PIN_MODE_INPUT(11) |                           \
                             PIN_MODE_ALTERNATE(GPIOB_SPI2_NSS) |           \
                             PIN_MODE_ALTERNATE(GPIOB_SPI2_SCK) |           \
                             PIN_MODE_ALTERNATE(GPIOB_SPI2_MISO) |          \
                             PIN_MODE_ALTERNATE(GPIOB_SPI2_MOSI))
#define VAL_GPIOB_OTYPER    (PIN_OTYPE_PUSHPULL(GPIOB_GYRO_NCS) |          \
                             PIN_OTYPE_PUSHPULL(GPIOB_PRS_NCS) |           \
                             PIN_OTYPE_PUSHPULL(GPIOB_FLASH_NCS))
#define VAL_GPIOB_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOB_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOB_ODR       (0xFFFFFFFF)
#define VAL_GPIOB_AFRL      (PIN_AFIO_AF(GPIOB_SWO, 0) |                    \
                             PIN_AFIO_AF(GPIOB_I2C1_SCL, 4) |               \
                             PIN_AFIO_AF(GPIOB_I2C1_SDA, 4))
#define VAL_GPIOB_AFRH      (PIN_AFIO_AF(GPIOB_SPI2_NSS, 5) |               \
                             PIN_AFIO_AF(GPIOB_SPI2_SCK, 5) |               \
                             PIN_AFIO_AF(GPIOB_SPI2_MISO, 5) |               \
                             PIN_AFIO_AF(GPIOB_SPI2_MOSI, 5))

/*
 * Port C setup.
 * All input with pull-up except:
 * PC0  - GPIOC_OTG_FS_POWER_ON (output push-pull).
 * PC7  - GPIOC_MCLK            (alternate 6).
 * PC10 - GPIOC_SCLK            (alternate 6).
 * PC12 - GPIOC_SDIN            (alternate 6).
 */

#define VAL_GPIOC_MODER     (PIN_MODE_INPUT(0) |                            \
                             PIN_MODE_INPUT(GPIOC_GYR_INT) |                \
                             PIN_MODE_INPUT(2) |                            \
                             PIN_MODE_INPUT(3) |                            \
                             PIN_MODE_ANALOG(GPIOC_BATT_MON) |              \
                             PIN_MODE_INPUT(GPIOC_BUTTON1) |                \
                             PIN_MODE_OUTPUT(GPIOC_REG_MODE) |              \
                             PIN_MODE_INPUT(GPIOC_REG_NCHG) |               \
                             PIN_MODE_INPUT(GPIOC_REG_NUSB_CONN) |          \
                             PIN_MODE_ALTERNATE(GPIOC_I2C3_SDA) |           \
                             PIN_MODE_ALTERNATE(GPIOC_SPI3_SCK) |           \
                             PIN_MODE_ALTERNATE(GPIOC_SPI3_MISO) |          \
                             PIN_MODE_ALTERNATE(GPIOC_SPI3_MOSI) |          \
                             PIN_MODE_INPUT(13) |                           \
                             PIN_MODE_INPUT(14) |                           \
                             PIN_MODE_INPUT(15))
#define VAL_GPIOC_OTYPER   (PIN_OTYPE_PUSHPULL(GPIOC_REG_MODE))
#define VAL_GPIOC_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOC_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(GPIOC_BUTTON1) |               \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(GPIOC_REG_NCHG) |              \
                             PIN_PUDR_PULLUP(GPIOC_REG_NUSB_CONN) |         \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOC_ODR       (0xFFFFFFFF &                                   \
                           ~(0))
#define VAL_GPIOC_AFRL      0
#define VAL_GPIOC_AFRH      (PIN_AFIO_AF(GPIOC_I2C3_SDA, 4) |                \
                             PIN_AFIO_AF(GPIOC_SPI3_SCK, 6) |                \
                             PIN_AFIO_AF(GPIOC_SPI3_MISO, 6) |                \
                             PIN_AFIO_AF(GPIOC_SPI3_MOSI, 6))

/*
 * Port D setup.
 * All input with pull-up except:
 * PD4  - GPIOD_RESET           (output push-pull).
 * PD5  - GPIOD_OVER_CURRENT    (input floating).
 * PD12 - GPIOD_LED4            (output push-pull).
 * PD13 - GPIOD_LED3            (output push-pull).
 * PD14 - GPIOD_LED5            (output push-pull).
 * PD15 - GPIOD_LED6            (output push-pull).
 */
#define VAL_GPIOD_MODER     (PIN_MODE_INPUT(0) |                            \
                             PIN_MODE_INPUT(1) |                            \
                             PIN_MODE_INPUT(2) |                            \
                             PIN_MODE_INPUT(3) |                            \
                             PIN_MODE_INPUT(4) |                            \
                             PIN_MODE_ALTERNATE(GPIOD_UART2_TX) |           \
                             PIN_MODE_ALTERNATE(GPIOD_UART2_RX) |           \
                             PIN_MODE_INPUT(7) |                            \
                             PIN_MODE_INPUT(8) |                            \
                             PIN_MODE_INPUT(9) |                            \
                             PIN_MODE_INPUT(10) |                           \
                             PIN_MODE_INPUT(11) |                           \
                             PIN_MODE_INPUT(12) |                  \
                             PIN_MODE_INPUT(13) |                  \
                             PIN_MODE_INPUT(14) |                  \
                             PIN_MODE_INPUT(15))
#define VAL_GPIOD_OTYPER    0x00000000
#define VAL_GPIOD_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOD_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOD_ODR       0x00000000
#define VAL_GPIOD_AFRL      (PIN_AFIO_AF(GPIOD_UART2_TX, 7) |               \
                             PIN_AFIO_AF(GPIOD_UART2_RX, 7))
#define VAL_GPIOD_AFRH      0x00000000

/*
 * Port E setup.
 * All input with pull-up except:
 * PE0  - GPIOE_INT1                (input floating).
 * PE1  - GPIOE_INT2                (input floating).
 * PE3  - GPIOE_CS_SPI              (output push-pull).
 */
#define VAL_GPIOE_MODER     (PIN_MODE_INPUT(GPIOE_ACC_INT1) |               \
                             PIN_MODE_INPUT(0) |                            \
                             PIN_MODE_OUTPUT(GPIOE_LED3) |                  \
                             PIN_MODE_OUTPUT(GPIOE_LED4) |                  \
                             PIN_MODE_OUTPUT(GPIOE_OLED_EN) |               \
                             PIN_MODE_OUTPUT(GPIOE_LED1) |                  \
                             PIN_MODE_INPUT(6) |                            \
                             PIN_MODE_INPUT(7) |                            \
                             PIN_MODE_INPUT(GPIOE_BUTTON4) |                \
                             PIN_MODE_INPUT(9) |                            \
                             PIN_MODE_INPUT(10) |                           \
                             PIN_MODE_INPUT(11) |                           \
                             PIN_MODE_OUTPUT(GPIOE_BT_NRESET) |             \
                             PIN_MODE_INPUT(13) |                           \
                             PIN_MODE_INPUT(14) |                           \
                             PIN_MODE_OUTPUT(GPIOE_CC1101_NCS))
#define VAL_GPIOE_OTYPER    0x00000000
#define VAL_GPIOE_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOE_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOE_ODR       0xFFFFFFFF
#define VAL_GPIOE_AFRL      0x00000000
#define VAL_GPIOE_AFRH      0x00000000

/*
 * Port F setup.
 * All input with pull-up.
 */
#define VAL_GPIOF_MODER     (PIN_MODE_INPUT(GPIOF_BUTTON2) |                \
                             PIN_MODE_INPUT(GPIOF_BUTTON3) |                \
                             PIN_MODE_OUTPUT(GPIOF_OLED_NCS))
#define VAL_GPIOF_OTYPER    0x00000000
#define VAL_GPIOF_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOF_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOF_ODR       0xFFFFFFFF
#define VAL_GPIOF_AFRL      0x00000000
#define VAL_GPIOF_AFRH      0x00000000

/*
 * Port G setup.
 * All input with pull-up.
 */
#define VAL_GPIOG_MODER     0x00000000
#define VAL_GPIOG_OTYPER    0x00000000
#define VAL_GPIOG_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOG_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOG_ODR       0xFFFFFFFF
#define VAL_GPIOG_AFRL      0x00000000
#define VAL_GPIOG_AFRH      0x00000000

/*
 * Port H setup.
 * All input with pull-up except:
 * PH0  - GPIOH_OSC_IN          (input floating).
 * PH1  - GPIOH_OSC_OUT         (input floating).
 */
#define GPIOH_OSC_IN            0
#define GPIOH_OSC_OUT           1
#define GPIOH_OLED_NRES         2
#define GPIOH_I2C2_SCL          4
#define GPIOH_I2C2_SDA          5

#define VAL_GPIOH_MODER     (PIN_MODE_INPUT(GPIOH_OSC_IN) |                 \
                             PIN_MODE_INPUT(GPIOH_OSC_OUT) |                \
                             PIN_MODE_OUTPUT(GPIOH_OLED_NRES) |             \
                             PIN_MODE_INPUT(3) |                            \
                             PIN_MODE_ALTERNATE(GPIOH_I2C2_SCL) |           \
                             PIN_MODE_ALTERNATE(GPIOH_I2C2_SDA) |           \
                             PIN_MODE_INPUT(6) |                            \
                             PIN_MODE_INPUT(7) |                            \
                             PIN_MODE_INPUT(8) |                            \
                             PIN_MODE_INPUT(9) |                            \
                             PIN_MODE_INPUT(10) |                           \
                             PIN_MODE_INPUT(11) |                           \
                             PIN_MODE_INPUT(12) |                           \
                             PIN_MODE_INPUT(13) |                           \
                             PIN_MODE_INPUT(14) |                           \
                             PIN_MODE_INPUT(15))
#define VAL_GPIOH_OTYPER    0x00000000
#define VAL_GPIOH_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOH_PUPDR     (PIN_PUDR_FLOATING(GPIOH_OSC_IN) |              \
                             PIN_PUDR_FLOATING(GPIOH_OSC_OUT) |             \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOH_ODR       0xFFFFFFFF
#define VAL_GPIOH_AFRL      (PIN_AFIO_AF(GPIOH_I2C2_SCL, 4) |               \
                             PIN_AFIO_AF(GPIOH_I2C2_SDA, 4))
#define VAL_GPIOH_AFRH      0x00000000

/*
 * Port I setup.
 * All input with pull-up.
 */
#define VAL_GPIOI_MODER     0x00000000
#define VAL_GPIOI_OTYPER    0x00000000
#define VAL_GPIOI_OSPEEDR   0xFFFFFFFF
#define VAL_GPIOI_PUPDR     (PIN_PUDR_PULLUP(0) |                           \
                             PIN_PUDR_PULLUP(1) |                           \
                             PIN_PUDR_PULLUP(2) |                           \
                             PIN_PUDR_PULLUP(3) |                           \
                             PIN_PUDR_PULLUP(4) |                           \
                             PIN_PUDR_PULLUP(5) |                           \
                             PIN_PUDR_PULLUP(6) |                           \
                             PIN_PUDR_PULLUP(7) |                           \
                             PIN_PUDR_PULLUP(8) |                           \
                             PIN_PUDR_PULLUP(9) |                           \
                             PIN_PUDR_PULLUP(10) |                          \
                             PIN_PUDR_PULLUP(11) |                          \
                             PIN_PUDR_PULLUP(12) |                          \
                             PIN_PUDR_PULLUP(13) |                          \
                             PIN_PUDR_PULLUP(14) |                          \
                             PIN_PUDR_PULLUP(15))
#define VAL_GPIOI_ODR       0xFFFFFFFF
#define VAL_GPIOI_AFRL      0x00000000
#define VAL_GPIOI_AFRH      0x00000000

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

//! @} @}

#endif /* _BOARD_H_ */
