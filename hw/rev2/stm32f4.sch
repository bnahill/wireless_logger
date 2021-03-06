EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:special
LIBS:analog_switches
LIBS:interface
LIBS:opto
LIBS:contrib
LIBS:wi_logger
LIBS:wireless_logger-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 7
Title "IMU Logger"
Date "24 sep 2013"
Rev "2"
Comp "McGill Integrated Microsystems Lab"
Comment1 "Ben Nahill (bnahill@gmail.com)"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L STM32F40X_BGA176 U11
U 1 1 51784288
P 3100 1500
F 0 "U11" H 3300 1100 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 2800 2000 50  0000 L CNN
F 2 "MODULE" H 2850 1000 50  0001 C CNN
F 3 "DOCUMENTATION" H 2800 900 50  0001 C CNN
	1    3100 1500
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 2 1 5178428E
P 5550 1500
F 0 "U11" H 5750 1100 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 5250 2000 50  0000 L CNN
F 2 "MODULE" H 5300 1000 50  0001 C CNN
F 3 "DOCUMENTATION" H 5250 900 50  0001 C CNN
	2    5550 1500
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 3 1 51784294
P 8050 1500
F 0 "U11" H 8250 1100 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 7750 2000 50  0000 L CNN
F 2 "MODULE" H 7800 1000 50  0001 C CNN
F 3 "DOCUMENTATION" H 7750 900 50  0001 C CNN
	3    8050 1500
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 4 1 5178429A
P 3100 3100
F 0 "U11" H 3300 2700 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 2850 3600 50  0000 L CNN
F 2 "MODULE" H 2850 2600 50  0001 C CNN
F 3 "DOCUMENTATION" H 2800 2500 50  0001 C CNN
	4    3100 3100
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 5 1 517842A0
P 5550 3100
F 0 "U11" H 5750 2700 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 5200 3600 50  0000 L CNN
F 2 "MODULE" H 5300 2600 50  0001 C CNN
F 3 "DOCUMENTATION" H 5250 2500 50  0001 C CNN
	5    5550 3100
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 6 1 517842A6
P 8050 3100
F 0 "U11" H 8250 2700 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 7700 3600 50  0000 L CNN
F 2 "MODULE" H 7800 2600 50  0001 C CNN
F 3 "DOCUMENTATION" H 7750 2500 50  0001 C CNN
	6    8050 3100
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 7 1 517842AC
P 2250 4550
F 0 "U11" H 2450 4150 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 1950 5050 50  0000 L CNN
F 2 "MODULE" H 2000 4050 50  0001 C CNN
F 3 "DOCUMENTATION" H 1950 3950 50  0001 C CNN
	7    2250 4550
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 8 1 517842B2
P 5550 4550
F 0 "U11" H 5750 4150 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 5250 5050 50  0000 L CNN
F 2 "MODULE" H 5300 4050 50  0001 C CNN
F 3 "DOCUMENTATION" H 5250 3950 50  0001 C CNN
	8    5550 4550
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 9 1 517842B8
P 8050 4550
F 0 "U11" H 8250 4150 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 7700 5050 50  0000 L CNN
F 2 "MODULE" H 7800 4050 50  0001 C CNN
F 3 "DOCUMENTATION" H 7750 3950 50  0001 C CNN
	9    8050 4550
	1    0    0    -1  
$EndComp
$Comp
L STM32F40X_BGA176 U11
U 10 1 517842BE
P 4700 6550
F 0 "U11" H 4900 6150 50  0000 C CNN
F 1 "STM32F40X_BGA176" H 4850 7100 50  0000 L CNN
F 2 "STM32F4_BGA176" H 4450 6050 50  0001 C CNN
F 3 "DOCUMENTATION" H 4400 5950 50  0001 C CNN
	10   4700 6550
	1    0    0    -1  
$EndComp
$Comp
L CAP C44
U 1 1 517842C9
P 5600 6850
F 0 "C44" H 5620 6820 50  0000 L TNN
F 1 "2.2u" H 5625 6870 30  0000 L BNN
F 2 "SMD0603" H 5600 6850 60  0001 C CNN
F 3 "" H 5600 6850 60  0000 C CNN
	1    5600 6850
	0    -1   -1   0   
$EndComp
$Comp
L CAP C43
U 1 1 517842CF
P 5350 6850
F 0 "C43" H 5370 6820 50  0000 L TNN
F 1 "2.2u" H 5375 6870 30  0000 L BNN
F 2 "SMD0603" H 5350 6850 60  0001 C CNN
F 3 "" H 5350 6850 60  0000 C CNN
	1    5350 6850
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR045
U 1 1 517842E4
P 4800 7350
F 0 "#PWR045" H 4800 7350 30  0001 C CNN
F 1 "GND" H 4800 7280 30  0001 C CNN
F 2 "" H 4800 7350 60  0000 C CNN
F 3 "" H 4800 7350 60  0000 C CNN
	1    4800 7350
	1    0    0    -1  
$EndComp
Text GLabel 4800 5700 1    60   Input ~ 0
V3
$Comp
L XTAL X5
U 1 1 518D46B6
P 9550 1150
F 0 "X5" H 9550 1090 40  0000 C TNN
F 1 "9HT10-32.768KBZF-T" V 9850 1100 40  0000 C BNN
F 2 "TXC_9HT10" H 9550 1150 60  0001 C CNN
F 3 "~" H 9550 1150 60  0000 C CNN
	1    9550 1150
	0    -1   -1   0   
$EndComp
$Comp
L CAP C59
U 1 1 518D46C5
P 9750 1000
F 0 "C59" H 9770 970 50  0000 L TNN
F 1 "9p" H 9775 1020 30  0000 L BNN
F 2 "SMD0402" H 9750 1000 60  0001 C CNN
F 3 "~" H 9750 1000 60  0000 C CNN
	1    9750 1000
	1    0    0    -1  
$EndComp
$Comp
L CAP C60
U 1 1 518D46D2
P 9750 1300
F 0 "C60" H 9770 1270 50  0000 L TNN
F 1 "9p" H 9775 1320 30  0000 L BNN
F 2 "SMD0402" H 9750 1300 60  0001 C CNN
F 3 "~" H 9750 1300 60  0000 C CNN
	1    9750 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR046
U 1 1 518D46DA
P 10050 1200
F 0 "#PWR046" H 10050 1200 30  0001 C CNN
F 1 "GND" H 10050 1130 30  0001 C CNN
F 2 "" H 10050 1200 60  0000 C CNN
F 3 "" H 10050 1200 60  0000 C CNN
	1    10050 1200
	1    0    0    -1  
$EndComp
$Comp
L CAP C45
U 1 1 518D498A
P 6100 5950
F 0 "C45" H 6120 5920 50  0000 L TNN
F 1 "2.2u" H 6125 5970 30  0000 L BNN
F 2 "0603" H 6100 5950 60  0001 C CNN
F 3 "~" H 6100 5950 60  0000 C CNN
	1    6100 5950
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR047
U 1 1 518D5148
P 7300 6250
F 0 "#PWR047" H 7300 6250 30  0001 C CNN
F 1 "GND" H 7300 6180 30  0001 C CNN
F 2 "" H 7300 6250 60  0000 C CNN
F 3 "" H 7300 6250 60  0000 C CNN
	1    7300 6250
	1    0    0    -1  
$EndComp
$Comp
L DIODESCH D1
U 1 1 518D51D1
P 3800 5450
F 0 "D1" H 3800 5550 40  0000 C CNN
F 1 "BAT43XV2" H 3800 5350 40  0000 C CNN
F 2 "SOD-523F" H 3800 5450 60  0001 C CNN
F 3 "~" H 3800 5450 60  0000 C CNN
	1    3800 5450
	0    1    1    0   
$EndComp
$Comp
L CP1 C40
U 1 1 518D5329
P 3900 6000
F 0 "C40" H 3950 6100 50  0000 L CNN
F 1 "DSK-3R3H224U-HL" V 4100 5900 50  0000 L CNN
F 2 "DSK-XXXXXX-HL" H 3900 6000 60  0001 C CNN
F 3 "~" H 3900 6000 60  0000 C CNN
	1    3900 6000
	1    0    0    -1  
$EndComp
$Comp
L CAP C39
U 1 1 518D535E
P 3650 6000
F 0 "C39" H 3670 5970 50  0000 L TNN
F 1 "10n" H 3675 6020 30  0000 L BNN
F 2 "SMD0402" H 3650 6000 60  0001 C CNN
F 3 "~" H 3650 6000 60  0000 C CNN
	1    3650 6000
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR048
U 1 1 518D53F7
P 3900 6400
F 0 "#PWR048" H 3900 6400 30  0001 C CNN
F 1 "GND" H 3900 6330 30  0001 C CNN
F 2 "" H 3900 6400 60  0000 C CNN
F 3 "" H 3900 6400 60  0000 C CNN
	1    3900 6400
	1    0    0    -1  
$EndComp
$Comp
L SWD_10PIN J4
U 1 1 518D5B5D
P 1950 5900
F 0 "J4" H 1970 5630 60  0000 L TNN
F 1 "SWD_10PIN" H 1920 6160 60  0000 C BNN
F 2 "~" H 1950 5900 60  0000 C CNN
F 3 "~" H 1950 5900 60  0000 C CNN
	1    1950 5900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR049
U 1 1 518D5B6C
P 1550 6200
F 0 "#PWR049" H 1550 6200 30  0001 C CNN
F 1 "GND" H 1550 6130 30  0001 C CNN
F 2 "" H 1550 6200 60  0000 C CNN
F 3 "" H 1550 6200 60  0000 C CNN
	1    1550 6200
	1    0    0    -1  
$EndComp
$Comp
L CAP C38
U 1 1 518D5DC2
P 1350 5900
F 0 "C38" H 1370 5870 50  0000 L TNN
F 1 "2.2u" H 1375 5920 30  0000 L BNN
F 2 "SMD0603" H 1350 5900 60  0001 C CNN
F 3 "~" H 1350 5900 60  0000 C CNN
	1    1350 5900
	0    -1   -1   0   
$EndComp
Text Label 2500 5900 2    60   ~ 0
SWO
Text Label 2500 5800 2    60   ~ 0
SWCLK
Text Label 2500 5700 2    60   ~ 0
SWDIO
Text Label 3800 1300 2    60   ~ 0
SWDIO
Text Label 3800 1200 2    60   ~ 0
SWCLK
Text Label 4950 1400 0    60   ~ 0
SWO
$Comp
L GND #PWR050
U 1 1 518D6E95
P 4150 6850
F 0 "#PWR050" H 4150 6850 30  0001 C CNN
F 1 "GND" H 4150 6780 30  0001 C CNN
F 2 "" H 4150 6850 60  0000 C CNN
F 3 "" H 4150 6850 60  0000 C CNN
	1    4150 6850
	1    0    0    -1  
$EndComp
Text GLabel 4150 6350 1    60   Input ~ 0
V3
$Comp
L GND #PWR051
U 1 1 518D71CA
P 3900 6850
F 0 "#PWR051" H 3900 6850 30  0001 C CNN
F 1 "GND" H 3900 6780 30  0001 C CNN
F 2 "" H 3900 6850 60  0000 C CNN
F 3 "" H 3900 6850 60  0000 C CNN
	1    3900 6850
	1    0    0    -1  
$EndComp
$Comp
L XTAL X4
U 1 1 518EB364
P 4050 4200
F 0 "X4" H 4050 4140 40  0000 C TNN
F 1 "XRCGB25M000F0L00R0" V 3850 4400 40  0000 C BNN
F 2 "~" H 4050 4200 60  0000 C CNN
F 3 "~" H 4050 4200 60  0000 C CNN
	1    4050 4200
	0    1    1    0   
$EndComp
$Comp
L CAP C42
U 1 1 518EB36A
P 3850 4350
F 0 "C42" H 3870 4320 50  0000 L TNN
F 1 "5p" H 3875 4370 30  0000 L BNN
F 2 "SMD0402" H 3850 4350 60  0001 C CNN
F 3 "~" H 3850 4350 60  0000 C CNN
	1    3850 4350
	-1   0    0    1   
$EndComp
$Comp
L CAP C41
U 1 1 518EB370
P 3850 4050
F 0 "C41" H 3870 4020 50  0000 L TNN
F 1 "5p" H 3875 4070 30  0000 L BNN
F 2 "SMD0402" H 3850 4050 60  0001 C CNN
F 3 "~" H 3850 4050 60  0000 C CNN
	1    3850 4050
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR052
U 1 1 518EB376
P 3550 4250
F 0 "#PWR052" H 3550 4250 30  0001 C CNN
F 1 "GND" H 3550 4180 30  0001 C CNN
F 2 "" H 3550 4250 60  0000 C CNN
F 3 "" H 3550 4250 60  0000 C CNN
	1    3550 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 4050 4250 4150
Wire Wire Line
	4250 4150 5250 4150
Wire Wire Line
	4250 4250 5250 4250
Wire Wire Line
	4250 4350 4250 4250
Connection ~ 4050 4350
Connection ~ 4050 4050
Wire Wire Line
	3950 4050 4250 4050
Wire Wire Line
	4050 4100 4050 4050
Wire Wire Line
	3950 4350 4250 4350
Wire Wire Line
	4050 4300 4050 4350
Connection ~ 3650 4200
Wire Wire Line
	3650 4200 3550 4200
Wire Wire Line
	3650 4050 3750 4050
Wire Wire Line
	3650 4050 3650 4350
Wire Wire Line
	3650 4350 3750 4350
Wire Wire Line
	3900 6550 4250 6550
Wire Wire Line
	3900 6850 3900 6550
Wire Wire Line
	4150 6450 4250 6450
Wire Wire Line
	4150 6350 4150 6450
Wire Wire Line
	4150 6750 4250 6750
Wire Wire Line
	4150 6850 4150 6750
Wire Wire Line
	5250 1400 4950 1400
Wire Wire Line
	3450 1200 3800 1200
Wire Wire Line
	3450 1300 3800 1300
Wire Wire Line
	2200 5900 2500 5900
Wire Wire Line
	2200 5800 2500 5800
Wire Wire Line
	2200 5700 2500 5700
Wire Wire Line
	1350 6000 1350 6100
Wire Wire Line
	1350 5600 1350 5800
Wire Wire Line
	1650 5700 1350 5700
Connection ~ 1550 5900
Wire Wire Line
	1550 5800 1650 5800
Connection ~ 1550 6100
Wire Wire Line
	1550 5900 1650 5900
Wire Wire Line
	2700 6100 2200 6100
Wire Wire Line
	1350 6100 1650 6100
Wire Wire Line
	1550 5800 1550 6200
Wire Wire Line
	3800 5250 3800 5200
Wire Wire Line
	4700 5750 8500 5750
Wire Wire Line
	3800 5200 4700 5200
Connection ~ 3900 5700
Wire Wire Line
	4600 5700 4600 5950
Connection ~ 3800 5700
Wire Wire Line
	3800 5700 3800 5650
Wire Wire Line
	3650 5700 4600 5700
Connection ~ 3900 6300
Wire Wire Line
	3650 6300 3650 6100
Wire Wire Line
	3900 6300 3650 6300
Wire Wire Line
	3900 6200 3900 6400
Wire Wire Line
	3650 5700 3650 5900
Wire Wire Line
	3900 5700 3900 5800
Connection ~ 4700 5750
Connection ~ 6300 6150
Wire Wire Line
	6300 6050 6300 6150
Connection ~ 6500 6150
Wire Wire Line
	6500 6150 6500 6050
Connection ~ 6700 6150
Wire Wire Line
	6700 6150 6700 6050
Connection ~ 6900 6150
Wire Wire Line
	6900 6150 6900 6050
Connection ~ 7100 6150
Wire Wire Line
	7100 6150 7100 6050
Connection ~ 7300 6150
Wire Wire Line
	7300 6050 7300 6250
Connection ~ 7500 6150
Wire Wire Line
	7500 6150 7500 6050
Connection ~ 7700 6150
Wire Wire Line
	7700 6150 7700 6050
Connection ~ 7900 6150
Wire Wire Line
	7900 6150 7900 6050
Connection ~ 8100 6150
Wire Wire Line
	8100 6150 8100 6050
Connection ~ 8300 6150
Wire Wire Line
	8300 6150 8300 6050
Wire Wire Line
	8500 6150 8500 6050
Wire Wire Line
	6100 6150 8500 6150
Wire Wire Line
	6100 6050 6100 6150
Connection ~ 6100 5750
Wire Wire Line
	6100 5750 6100 5850
Connection ~ 6300 5750
Wire Wire Line
	6300 5750 6300 5850
Connection ~ 6500 5750
Wire Wire Line
	6500 5750 6500 5850
Connection ~ 6700 5750
Wire Wire Line
	6700 5750 6700 5850
Connection ~ 6900 5750
Wire Wire Line
	6900 5750 6900 5850
Connection ~ 7100 5750
Wire Wire Line
	7100 5750 7100 5850
Connection ~ 7300 5750
Wire Wire Line
	7300 5750 7300 5850
Connection ~ 7500 5750
Wire Wire Line
	7500 5750 7500 5850
Connection ~ 7700 5750
Wire Wire Line
	7700 5750 7700 5850
Connection ~ 7900 5750
Wire Wire Line
	7900 5750 7900 5850
Connection ~ 8100 5750
Wire Wire Line
	8100 5750 8100 5850
Connection ~ 8300 5750
Wire Wire Line
	8300 5750 8300 5850
Connection ~ 5650 5750
Wire Wire Line
	8500 5750 8500 5850
Connection ~ 9550 1000
Wire Wire Line
	9350 1100 9350 1000
Wire Wire Line
	8400 1100 9350 1100
Connection ~ 9550 1300
Wire Wire Line
	9350 1200 8400 1200
Wire Wire Line
	9350 1300 9350 1200
Wire Wire Line
	9350 1300 9650 1300
Wire Wire Line
	9550 1250 9550 1300
Wire Wire Line
	9350 1000 9650 1000
Wire Wire Line
	9550 1050 9550 1000
Connection ~ 9950 1150
Wire Wire Line
	10050 1150 10050 1200
Wire Wire Line
	9950 1150 10050 1150
Wire Wire Line
	9950 1300 9850 1300
Wire Wire Line
	9950 1000 9950 1300
Wire Wire Line
	9850 1000 9950 1000
Connection ~ 5600 7200
Wire Wire Line
	5750 7200 5750 6350
Wire Wire Line
	5750 6350 5250 6350
Connection ~ 4800 5750
Connection ~ 5350 7200
Wire Wire Line
	5600 7200 5600 6950
Connection ~ 4800 7200
Wire Wire Line
	4700 7200 4700 7100
Wire Wire Line
	4800 7100 4800 7350
Wire Wire Line
	4700 7200 5750 7200
Wire Wire Line
	5350 7200 5350 6950
Wire Wire Line
	5350 6650 5350 6750
Wire Wire Line
	5250 6650 5350 6650
Wire Wire Line
	5600 6550 5250 6550
Wire Wire Line
	5600 6750 5600 6550
Wire Wire Line
	4700 5200 4700 5950
Wire Wire Line
	4800 5700 4800 5950
Wire Wire Line
	5650 6250 5650 5750
Wire Wire Line
	5250 6250 5650 6250
Wire Wire Line
	3550 4200 3550 4250
Text HLabel 2600 1600 0    60   Output ~ 0
SPI1_CLK
Text HLabel 2600 1800 0    60   Output ~ 0
SPI1_MOSI
Text HLabel 2600 1700 0    60   Input ~ 0
SPI1_MISO
Text HLabel 6150 1300 2    60   Output ~ 0
SPI2_CLK
Text HLabel 6150 1100 2    60   Output ~ 0
SPI2_MOSI
Text HLabel 6150 1200 2    60   Input ~ 0
SPI2_MISO
Text HLabel 8500 1600 2    60   Output ~ 0
SPI3_CLK
Text HLabel 8500 1400 2    60   Output ~ 0
SPI3_MOSI
Text HLabel 8500 1500 2    60   Input ~ 0
SPI3_MISO
Wire Wire Line
	2600 1800 2800 1800
Wire Wire Line
	2800 1700 2600 1700
Wire Wire Line
	2600 1600 2800 1600
Text HLabel 6250 1800 2    60   BiDi ~ 0
I2C1_SCL
Text HLabel 6250 1700 2    60   BiDi ~ 0
I2C1_SDA
Text HLabel 4750 4550 0    60   BiDi ~ 0
I2C2_SCL
Text HLabel 4750 4650 0    60   BiDi ~ 0
I2C2_SDA
Text HLabel 3800 1800 2    60   BiDi ~ 0
I2C3_SCL
Text HLabel 9600 1700 2    60   BiDi ~ 0
I2C3_SDA
Wire Wire Line
	5900 1800 6250 1800
Wire Wire Line
	5900 1700 6250 1700
Text HLabel 3800 1400 2    60   BiDi ~ 0
USB_D+
Text HLabel 3800 1500 2    60   BiDi ~ 0
USB_D-
Wire Wire Line
	3800 1500 3450 1500
Wire Wire Line
	3450 1600 3800 1600
Wire Wire Line
	3800 1400 3450 1400
Text HLabel 3800 1600 2    60   BiDi ~ 0
USB_ID
Wire Wire Line
	3800 1700 3450 1700
Wire Wire Line
	6150 1300 5900 1300
Wire Wire Line
	5900 1200 6150 1200
Wire Wire Line
	6150 1100 5900 1100
$Comp
L RES R26
U 1 1 518ECA23
P 6150 2000
F 0 "R26" H 6220 1965 50  0000 L TNN
F 1 "10k" H 6150 2055 30  0000 C BNN
F 2 "SMD0402" H 6150 2000 60  0001 C CNN
F 3 "~" H 6150 2000 60  0000 C CNN
	1    6150 2000
	0    -1   -1   0   
$EndComp
$Comp
L RES R25
U 1 1 518ECA30
P 6000 2000
F 0 "R25" H 6070 1965 50  0000 L TNN
F 1 "10k" H 6000 2055 30  0000 C BNN
F 2 "SMD0402" H 6000 2000 60  0001 C CNN
F 3 "~" H 6000 2000 60  0000 C CNN
	1    6000 2000
	0    -1   -1   0   
$EndComp
Text GLabel 6350 2150 1    60   Input ~ 0
V3
Wire Wire Line
	6350 2200 6350 2150
Wire Wire Line
	6000 2200 6350 2200
Wire Wire Line
	6000 2200 6000 2150
Wire Wire Line
	6150 2200 6150 2150
Connection ~ 6150 2200
Wire Wire Line
	6150 1850 6150 1800
Connection ~ 6150 1800
Wire Wire Line
	6000 1850 6000 1700
Connection ~ 6000 1700
Text GLabel 4600 5050 1    60   Input ~ 0
V3
$Comp
L RES R23
U 1 1 518ECFB7
P 4850 4900
F 0 "R23" H 4920 4865 50  0000 L TNN
F 1 "10k" H 4850 4955 30  0000 C BNN
F 2 "SMD0402" H 4850 4900 60  0001 C CNN
F 3 "~" H 4850 4900 60  0000 C CNN
	1    4850 4900
	0    -1   -1   0   
$EndComp
$Comp
L RES R24
U 1 1 518ECFC4
P 5000 4900
F 0 "R24" H 5070 4865 50  0000 L TNN
F 1 "10k" H 5000 4955 30  0000 C BNN
F 2 "SMD0402" H 5000 4900 60  0001 C CNN
F 3 "~" H 5000 4900 60  0000 C CNN
	1    5000 4900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 4650 5250 4650
Wire Wire Line
	4750 4550 5250 4550
Wire Wire Line
	4850 4750 4850 4650
Connection ~ 4850 4650
Wire Wire Line
	5000 4750 5000 4550
Connection ~ 5000 4550
Wire Wire Line
	5000 5100 5000 5050
Wire Wire Line
	4600 5100 5000 5100
Wire Wire Line
	4600 5100 4600 5050
Wire Wire Line
	4850 5050 4850 5100
Connection ~ 4850 5100
Wire Wire Line
	8500 1600 8400 1600
Wire Wire Line
	8500 1500 8400 1500
Wire Wire Line
	8400 1400 8500 1400
Text Notes 9250 1550 0    60   ~ 0
FLASH
Text Notes 6050 950  0    60   ~ 0
GYRO, CC1101,\nPRESSURE
Text Notes 1750 1750 0    60   ~ 0
OLED
Text HLabel 7500 1600 0    60   BiDi ~ 0
EXTI5
Text HLabel 2500 3400 0    60   BiDi ~ 0
EXTI7
Text HLabel 6150 3400 2    60   BiDi ~ 0
EXTI8
Text GLabel 3800 1700 2    60   Input ~ 0
Vbus
$Comp
L PUSHBUTTON PB5
U 1 1 51929B1F
P 3100 6950
F 0 "PB5" H 3250 6975 40  0000 L BNN
F 1 "KMT0" H 3100 6925 40  0000 C TNN
F 2 "~" H 3100 6950 60  0000 C CNN
F 3 "~" H 3100 6950 60  0000 C CNN
	1    3100 6950
	0    -1   -1   0   
$EndComp
$Comp
L RES R22
U 1 1 51929B42
P 2700 6400
F 0 "R22" H 2770 6365 50  0000 L TNN
F 1 "10k" H 2700 6455 30  0000 C BNN
F 2 "SMD0402" H 2700 6400 60  0001 C CNN
F 3 "~" H 2700 6400 60  0000 C CNN
	1    2700 6400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2700 6250 2700 6100
Wire Wire Line
	2700 6550 2700 6650
Wire Wire Line
	2700 6650 4250 6650
Wire Wire Line
	3100 6650 3100 6750
Connection ~ 3100 6650
$Comp
L GND #PWR053
U 1 1 51929D6E
P 3100 7200
F 0 "#PWR053" H 3100 7200 30  0001 C CNN
F 1 "GND" H 3100 7130 30  0001 C CNN
F 2 "" H 3100 7200 60  0000 C CNN
F 3 "" H 3100 7200 60  0000 C CNN
	1    3100 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 7150 3100 7200
Text HLabel 2500 3300 0    60   Output ~ 0
FLASH_nCS
Text HLabel 8600 3200 2    60   Output ~ 0
OLED_nCS
Text HLabel 2600 1400 0    60   Output ~ 0
OLED_nD/C
Text HLabel 4900 3100 0    60   Output ~ 0
OLED_EN
Text HLabel 4850 4350 0    60   Output ~ 0
OLED_nRES
Text HLabel 4900 2700 0    60   BiDi ~ 0
EXTI0
Text HLabel 7500 1200 0    60   BiDi ~ 0
EXTI1
Text HLabel 4950 1300 0    60   BiDi ~ 0
EXTI2
Text HLabel 1700 4100 0    60   BiDi ~ 0
EXTI3
Text HLabel 1700 4200 0    60   BiDi ~ 0
EXTI4
Text HLabel 2500 3200 0    60   Output ~ 0
PRS_nCS
Text HLabel 4950 1600 0    60   Output ~ 0
GYRO_nCS
Text HLabel 2600 1200 0    60   Input ~ 0
UART4_RX
Text HLabel 2600 1100 0    60   Output ~ 0
UART4_TX
Wire Wire Line
	2600 1100 2800 1100
Wire Wire Line
	2800 1200 2600 1200
$Comp
L LED D2
U 1 1 5194AEBE
P 9500 4600
F 0 "D2" H 9500 4700 50  0000 C CNN
F 1 "LS Q976-NR-1" H 9500 4500 50  0000 C CNN
F 2 "SMD0603" H 9500 4600 60  0001 C CNN
F 3 "~" H 9500 4600 60  0000 C CNN
	1    9500 4600
	0    1    1    0   
$EndComp
$Comp
L LED D3
U 1 1 5194AECB
P 9800 4600
F 0 "D3" H 9800 4700 50  0000 C CNN
F 1 "LW Q38G-Q1S1-3K6L-1" H 9800 4500 50  0000 C CNN
F 2 "SMD0603" H 9800 4600 60  0001 C CNN
F 3 "~" H 9800 4600 60  0000 C CNN
	1    9800 4600
	0    1    1    0   
$EndComp
$Comp
L LED D4
U 1 1 5194AED1
P 10100 4600
F 0 "D4" H 10100 4700 50  0000 C CNN
F 1 "LB Q39E-N1P1-35-1" H 10100 4500 50  0000 C CNN
F 2 "SMD0603" H 10100 4600 60  0001 C CNN
F 3 "~" H 10100 4600 60  0000 C CNN
	1    10100 4600
	0    1    1    0   
$EndComp
$Comp
L RES R27
U 1 1 5194AED9
P 9500 4000
F 0 "R27" H 9570 3965 50  0000 L TNN
F 1 "RES" H 9500 4055 30  0000 C BNN
F 2 "SMD0402" H 9500 4000 60  0001 C CNN
F 3 "~" H 9500 4000 60  0000 C CNN
	1    9500 4000
	0    -1   -1   0   
$EndComp
$Comp
L RES R28
U 1 1 5194AEE6
P 9800 4000
F 0 "R28" H 9870 3965 50  0000 L TNN
F 1 "RES" H 9800 4055 30  0000 C BNN
F 2 "SMD0402" H 9800 4000 60  0001 C CNN
F 3 "~" H 9800 4000 60  0000 C CNN
	1    9800 4000
	0    -1   -1   0   
$EndComp
$Comp
L RES R29
U 1 1 5194AEEC
P 10100 4000
F 0 "R29" H 10170 3965 50  0000 L TNN
F 1 "RES" H 10100 4055 30  0000 C BNN
F 2 "SMD0402" H 10100 4000 60  0001 C CNN
F 3 "~" H 10100 4000 60  0000 C CNN
	1    10100 4000
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR054
U 1 1 5194AF12
P 9800 5050
F 0 "#PWR054" H 9800 5050 30  0001 C CNN
F 1 "GND" H 9800 4980 30  0001 C CNN
F 2 "" H 9800 5050 60  0000 C CNN
F 3 "" H 9800 5050 60  0000 C CNN
	1    9800 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 4800 9800 5050
Wire Wire Line
	9500 4900 10100 4900
Wire Wire Line
	10100 4900 10100 4800
Connection ~ 9800 4900
Wire Wire Line
	9500 4900 9500 4800
Wire Wire Line
	9800 4400 9800 4150
Wire Wire Line
	10100 4150 10100 4400
Wire Wire Line
	9500 4400 9500 4150
Text HLabel 6150 3000 2    60   Output ~ 0
BT_NRESET
Text HLabel 6150 2700 2    60   Output ~ 0
CC1101_nCS
Text HLabel 6150 1400 2    60   Input ~ 0
SPI2_NSS
Wire Wire Line
	6150 1400 5900 1400
Wire Wire Line
	7500 1600 7750 1600
Text HLabel 7500 3300 0    60   BiDi ~ 0
EXTI6
Wire Wire Line
	7500 3300 7750 3300
Wire Wire Line
	2500 3400 2800 3400
Wire Wire Line
	6150 3400 5900 3400
Text Notes 9500 5150 3    60   ~ 0
RED
Text Notes 9800 5150 3    60   ~ 0
WHITE
Text Notes 10100 5150 3    60   ~ 0
BLUE
$Comp
L CAP C50
U 1 1 51956287
P 7100 5950
F 0 "C50" H 7120 5920 50  0000 L TNN
F 1 "100n" H 7125 5970 30  0000 L BNN
F 2 "0603" H 7100 5950 60  0001 C CNN
F 3 "~" H 7100 5950 60  0000 C CNN
	1    7100 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C56
U 1 1 519562A4
P 8100 5950
F 0 "C56" H 8120 5920 50  0000 L TNN
F 1 "10n" H 8125 5970 30  0000 L BNN
F 2 "0402" H 8100 5950 60  0001 C CNN
F 3 "~" H 8100 5950 60  0000 C CNN
	1    8100 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C58
U 1 1 519562B0
P 8500 5950
F 0 "C58" H 8520 5920 50  0000 L TNN
F 1 "10n" H 8525 5970 30  0000 L BNN
F 2 "0402" H 8500 5950 60  0001 C CNN
F 3 "~" H 8500 5950 60  0000 C CNN
	1    8500 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C54
U 1 1 519562BD
P 7900 5950
F 0 "C54" H 7920 5920 50  0000 L TNN
F 1 "10n" H 7925 5970 30  0000 L BNN
F 2 "0402" H 7900 5950 60  0001 C CNN
F 3 "~" H 7900 5950 60  0000 C CNN
	1    7900 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C51
U 1 1 519562E7
P 7300 5950
F 0 "C51" H 7320 5920 50  0000 L TNN
F 1 "100n" H 7325 5970 30  0000 L BNN
F 2 "0603" H 7300 5950 60  0001 C CNN
F 3 "~" H 7300 5950 60  0000 C CNN
	1    7300 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C52
U 1 1 519562ED
P 7500 5950
F 0 "C52" H 7520 5920 50  0000 L TNN
F 1 "100n" H 7525 5970 30  0000 L BNN
F 2 "0603" H 7500 5950 60  0001 C CNN
F 3 "~" H 7500 5950 60  0000 C CNN
	1    7500 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C53
U 1 1 519562F3
P 7700 5950
F 0 "C53" H 7720 5920 50  0000 L TNN
F 1 "100n" H 7725 5970 30  0000 L BNN
F 2 "0603" H 7700 5950 60  0001 C CNN
F 3 "~" H 7700 5950 60  0000 C CNN
	1    7700 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C46
U 1 1 519562FE
P 6300 5950
F 0 "C46" H 6320 5920 50  0000 L TNN
F 1 "2.2u" H 6325 5970 30  0000 L BNN
F 2 "0603" H 6300 5950 60  0001 C CNN
F 3 "~" H 6300 5950 60  0000 C CNN
	1    6300 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C47
U 1 1 51956304
P 6500 5950
F 0 "C47" H 6520 5920 50  0000 L TNN
F 1 "2.2u" H 6525 5970 30  0000 L BNN
F 2 "0603" H 6500 5950 60  0001 C CNN
F 3 "~" H 6500 5950 60  0000 C CNN
	1    6500 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C48
U 1 1 5195630A
P 6700 5950
F 0 "C48" H 6720 5920 50  0000 L TNN
F 1 "2.2u" H 6725 5970 30  0000 L BNN
F 2 "0603" H 6700 5950 60  0001 C CNN
F 3 "~" H 6700 5950 60  0000 C CNN
	1    6700 5950
	0    -1   -1   0   
$EndComp
$Comp
L CAP C49
U 1 1 51956310
P 6900 5950
F 0 "C49" H 6920 5920 50  0000 L TNN
F 1 "2.2u" H 6925 5970 30  0000 L BNN
F 2 "0603" H 6900 5950 60  0001 C CNN
F 3 "~" H 6900 5950 60  0000 C CNN
	1    6900 5950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4900 2700 5250 2700
Wire Wire Line
	7500 1200 7750 1200
Wire Wire Line
	4950 1300 5250 1300
Text HLabel 3550 3200 2    60   Output ~ 0
REG_MODE
Text HLabel 3550 2800 2    60   Input ~ 0
REG_nCHG
Text HLabel 3550 2900 2    60   Input ~ 0
REG_nUSB_CONN
Wire Wire Line
	3550 3200 3450 3200
Wire Wire Line
	3550 2800 3450 2800
Wire Wire Line
	3550 2900 3450 2900
Text HLabel 1400 3300 0    60   Input ~ 0
UART2_RX
Text HLabel 1400 3200 0    60   Output ~ 0
UART2_TX
Wire Wire Line
	1400 3200 1600 3200
Wire Wire Line
	1600 3300 1400 3300
Wire Wire Line
	3450 1800 3800 1800
Wire Wire Line
	8400 1700 9600 1700
$Comp
L RES R35
U 1 1 51A40C1B
P 9450 1950
F 0 "R35" H 9520 1915 50  0000 L TNN
F 1 "10k" H 9450 2005 30  0000 C BNN
F 2 "SMD0402" H 9450 1950 60  0001 C CNN
F 3 "~" H 9450 1950 60  0000 C CNN
	1    9450 1950
	0    -1   -1   0   
$EndComp
Text GLabel 9250 2100 1    60   Input ~ 0
V3
Wire Wire Line
	9250 2100 9250 2200
Wire Wire Line
	9250 2200 9450 2200
Wire Wire Line
	9450 2200 9450 2100
Wire Wire Line
	9450 1800 9450 1700
Connection ~ 9450 1700
$Comp
L RES R34
U 1 1 51A40E38
P 3700 2050
F 0 "R34" H 3770 2015 50  0000 L TNN
F 1 "10k" H 3700 2105 30  0000 C BNN
F 2 "SMD0402" H 3700 2050 60  0001 C CNN
F 3 "~" H 3700 2050 60  0000 C CNN
	1    3700 2050
	0    -1   -1   0   
$EndComp
Text GLabel 3500 2200 1    60   Input ~ 0
V3
Wire Wire Line
	3500 2200 3500 2300
Wire Wire Line
	3500 2300 3700 2300
Wire Wire Line
	3700 2300 3700 2200
Wire Wire Line
	3700 1900 3700 1800
Connection ~ 3700 1800
Text GLabel 1350 5600 1    60   Input ~ 0
V3
Connection ~ 1350 5700
Wire Wire Line
	9500 3850 9500 3650
Wire Wire Line
	9800 3850 9800 3650
Wire Wire Line
	10100 3850 10100 3650
Text Label 9500 3750 1    60   ~ 0
LED1
Text Label 9800 3750 1    60   ~ 0
LED2
Text Label 10100 3750 1    60   ~ 0
LED3
Text Label 5150 3200 2    60   ~ 0
LED1
Wire Wire Line
	5150 3000 5250 3000
Text Label 5150 3000 2    60   ~ 0
LED2
Wire Wire Line
	5150 2900 5250 2900
Wire Wire Line
	6150 3000 5900 3000
Wire Wire Line
	6150 2700 5900 2700
Text Label 5150 2900 2    60   ~ 0
LED3
Wire Wire Line
	5150 3200 5250 3200
Wire Wire Line
	4900 3100 5250 3100
Wire Wire Line
	2500 3300 2800 3300
Wire Wire Line
	4950 1600 5250 1600
Wire Wire Line
	2600 1400 2800 1400
Text Notes 3200 6950 0    60   ~ 0
Reset button
Wire Wire Line
	4850 4350 5250 4350
Wire Wire Line
	2500 3200 2800 3200
Wire Wire Line
	8600 3200 8400 3200
$Comp
L RES R37
U 1 1 51DD5A64
P 6850 1000
F 0 "R37" H 6920 965 50  0000 L TNN
F 1 "1M" H 6850 1055 30  0000 C BNN
F 2 "SMD0402" H 6850 1000 60  0001 C CNN
F 3 "~" H 6850 1000 60  0000 C CNN
	1    6850 1000
	0    -1   -1   0   
$EndComp
$Comp
L RES R38
U 1 1 51DD5A7C
P 6850 1400
F 0 "R38" H 6920 1365 50  0000 L TNN
F 1 "1M" H 6850 1455 30  0000 C BNN
F 2 "SMD0402" H 6850 1400 60  0001 C CNN
F 3 "~" H 6850 1400 60  0000 C CNN
	1    6850 1400
	0    -1   -1   0   
$EndComp
$Comp
L CAP C71
U 1 1 51DD5A93
P 7050 1450
F 0 "C71" H 7070 1420 50  0000 L TNN
F 1 "1u" H 7075 1470 30  0000 L BNN
F 2 "~" H 7050 1450 60  0000 C CNN
F 3 "~" H 7050 1450 60  0000 C CNN
	1    7050 1450
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR055
U 1 1 51DD5ABA
P 6850 1650
F 0 "#PWR055" H 6850 1650 30  0001 C CNN
F 1 "GND" H 6850 1580 30  0001 C CNN
F 2 "" H 6850 1650 60  0000 C CNN
F 3 "" H 6850 1650 60  0000 C CNN
	1    6850 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1550 6850 1650
Wire Wire Line
	6850 1600 7050 1600
Wire Wire Line
	7050 1600 7050 1550
Connection ~ 6850 1600
Wire Wire Line
	6850 1150 6850 1250
Wire Wire Line
	6850 1200 7050 1200
Wire Wire Line
	7050 1200 7050 1350
Connection ~ 6850 1200
Text GLabel 6850 750  1    60   Input ~ 0
Vbat
Wire Wire Line
	6850 750  6850 850 
Wire Wire Line
	7050 1300 7250 1300
Wire Wire Line
	7250 1300 7250 1500
Connection ~ 7050 1300
Wire Wire Line
	7250 1500 7750 1500
Text HLabel 7500 1700 0    60   Output ~ 0
USART6_TX
Wire Wire Line
	7500 1700 7750 1700
Text HLabel 7500 1800 0    60   Input ~ 0
USART6_RX
Wire Wire Line
	7500 1800 7750 1800
Text HLabel 4950 1700 0    60   Output ~ 0
USART1_TX
Text HLabel 4950 1800 0    60   Output ~ 0
USART1_RX
Wire Wire Line
	4950 1800 5250 1800
Wire Wire Line
	4950 1700 5250 1700
$Comp
L CAP C57
U 1 1 519562AA
P 8300 5950
F 0 "C57" H 8320 5920 50  0000 L TNN
F 1 "10n" H 8325 5970 30  0000 L BNN
F 2 "0402" H 8300 5950 60  0001 C CNN
F 3 "~" H 8300 5950 60  0000 C CNN
	1    8300 5950
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
