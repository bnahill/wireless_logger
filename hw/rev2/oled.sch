EESchema Schematic File Version 2  date Sat 01 Jun 2013 04:42:25 PM EDT
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
Sheet 6 7
Title "IMU Logger"
Date "1 jun 2013"
Rev "2"
Comp "McGill Integrated Microsystems Lab"
Comment1 "Ben Nahill (bnahill@gmail.com)"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RES R20
U 1 1 505F5E33
P 4500 4750
F 0 "R20" H 4570 4715 50  0000 L TNN
F 1 "100k" H 4500 4805 30  0000 C BNN
F 2 "" H 4500 4750 60  0001 C CNN
F 3 "" H 4500 4750 60  0001 C CNN
	1    4500 4750
	0    -1   -1   0   
$EndComp
$Comp
L CAP C33
U 1 1 504A0E3E
P 4800 4550
F 0 "C33" H 4820 4520 50  0000 L TNN
F 1 "2.2u" H 4825 4570 30  0000 L BNN
F 2 "" H 4800 4550 60  0001 C CNN
F 3 "" H 4800 4550 60  0001 C CNN
	1    4800 4550
	0    -1   -1   0   
$EndComp
Text HLabel 5300 4550 0    60   Input ~ 0
SDIN
Text HLabel 5300 4450 0    60   Input ~ 0
SCLK
Text HLabel 5300 4350 0    60   Input ~ 0
nD/C
Text HLabel 4300 4250 0    60   Input ~ 0
nRES
Text HLabel 5300 4150 0    60   Input ~ 0
nCS
$Comp
L SSD1306_SER J3
U 1 1 504A0D79
P 5850 4150
F 0 "J3" H 5950 3350 60  0000 C CNN
F 1 "SSD1306_SER" H 5900 4950 60  0000 C CNN
F 2 "" H 5850 4150 60  0001 C CNN
F 3 "" H 5850 4150 60  0001 C CNN
	1    5850 4150
	1    0    0    -1  
$EndComp
$Comp
L CAP C37
U 1 1 504A0CF7
P 5550 5050
F 0 "C37" H 5570 5020 50  0000 L TNN
F 1 "2.2u" H 5575 5070 30  0000 L BNN
F 2 "" H 5550 5050 60  0001 C CNN
F 3 "" H 5550 5050 60  0001 C CNN
	1    5550 5050
	0    -1   -1   0   
$EndComp
$Comp
L CAP C36
U 1 1 504A0CF4
P 5350 5050
F 0 "C36" H 5370 5020 50  0000 L TNN
F 1 "2.2u" H 5375 5070 30  0000 L BNN
F 2 "" H 5350 5050 60  0001 C CNN
F 3 "" H 5350 5050 60  0001 C CNN
	1    5350 5050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR041
U 1 1 504A0CF1
P 5050 6200
F 0 "#PWR041" H 5050 6200 30  0001 C CNN
F 1 "GND" H 5050 6130 30  0001 C CNN
F 2 "" H 5050 6200 60  0001 C CNN
F 3 "" H 5050 6200 60  0001 C CNN
	1    5050 6200
	1    0    0    -1  
$EndComp
$Comp
L RES R21
U 1 1 504A0CDB
P 5150 5000
F 0 "R21" H 5220 4965 50  0000 L TNN
F 1 "390k" H 5150 5055 30  0000 C BNN
F 2 "" H 5150 5000 60  0001 C CNN
F 3 "" H 5150 5000 60  0001 C CNN
	1    5150 5000
	0    -1   -1   0   
$EndComp
$Comp
L CAP C35
U 1 1 504A0A07
P 5350 3650
F 0 "C35" H 5370 3620 50  0000 L TNN
F 1 "1u" H 5375 3670 30  0000 L BNN
F 2 "" H 5350 3650 60  0001 C CNN
F 3 "" H 5350 3650 60  0001 C CNN
	1    5350 3650
	-1   0    0    1   
$EndComp
$Comp
L CAP C34
U 1 1 504A09F8
P 5350 3450
F 0 "C34" H 5370 3420 50  0000 L TNN
F 1 "1u" H 5375 3470 30  0000 L BNN
F 2 "" H 5350 3450 60  0001 C CNN
F 3 "" H 5350 3450 60  0001 C CNN
	1    5350 3450
	-1   0    0    1   
$EndComp
$Comp
L MOS_N Q1
U 1 1 518F8DBC
P 4950 5700
F 0 "Q1" H 4960 5870 60  0000 R CNN
F 1 "2N7002P" H 4960 5550 60  0000 R CNN
F 2 "SOT23GDS" H 4950 5700 60  0001 C CNN
F 3 "~" H 4950 5700 60  0000 C CNN
	1    4950 5700
	1    0    0    -1  
$EndComp
Connection ~ 4800 3850
Wire Wire Line
	4800 4450 4800 3850
Wire Wire Line
	5300 4150 5650 4150
Wire Wire Line
	5300 4350 5650 4350
Wire Wire Line
	5300 4550 5650 4550
Wire Wire Line
	5300 4050 5650 4050
Wire Wire Line
	5650 4850 5550 4850
Wire Wire Line
	5550 4850 5550 4950
Wire Wire Line
	5650 4650 5150 4650
Wire Wire Line
	5150 4650 5150 4850
Connection ~ 5350 5300
Wire Wire Line
	5550 5300 5550 5150
Wire Wire Line
	5650 3750 5150 3750
Wire Wire Line
	5150 3750 5150 3650
Wire Wire Line
	5150 3650 5250 3650
Wire Wire Line
	5650 3550 5150 3550
Wire Wire Line
	5150 3550 5150 3450
Wire Wire Line
	5150 3450 5250 3450
Wire Wire Line
	5650 3450 5450 3450
Wire Wire Line
	5650 3650 5450 3650
Wire Wire Line
	5150 5300 5150 5150
Wire Wire Line
	5350 4950 5350 4750
Wire Wire Line
	5350 4750 5650 4750
Wire Wire Line
	4950 3950 5650 3950
Connection ~ 5150 5300
Wire Wire Line
	4700 3850 5650 3850
Wire Wire Line
	5650 4450 5300 4450
Wire Wire Line
	4300 4250 5650 4250
Wire Wire Line
	4800 5300 4800 4650
Wire Wire Line
	4500 4600 4500 4250
Connection ~ 4500 4250
Wire Wire Line
	5350 5300 5350 5150
Wire Wire Line
	5050 5300 5050 5500
Connection ~ 5050 5300
Wire Wire Line
	5050 5900 5050 6200
Text HLabel 4050 5700 0    60   Input ~ 0
EN
Wire Wire Line
	4500 5300 5550 5300
Wire Wire Line
	4050 5700 5200 5700
$Comp
L MOS_N Q2
U 1 1 518F9B5D
P 5400 5700
F 0 "Q2" H 5410 5870 60  0000 R CNN
F 1 "2N7002P" H 5410 5550 60  0000 R CNN
F 2 "SOT23GDS" H 5400 5700 60  0001 C CNN
F 3 "~" H 5400 5700 60  0000 C CNN
	1    5400 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 5500 5500 5300
Connection ~ 5500 5300
Wire Wire Line
	5500 5900 5500 6000
Wire Wire Line
	5500 6000 5050 6000
Connection ~ 5050 6000
Connection ~ 4750 5700
Text GLabel 4700 3850 0    60   Input ~ 0
Vbat
Text GLabel 5300 4050 0    60   Input ~ 0
V3
Wire Wire Line
	4950 3950 4950 5300
Connection ~ 4950 5300
$Comp
L RES R19
U 1 1 5194609C
P 4300 5900
F 0 "R19" H 4370 5865 50  0000 L TNN
F 1 "100k" H 4300 5955 30  0000 C BNN
F 2 "" H 4300 5900 60  0001 C CNN
F 3 "" H 4300 5900 60  0001 C CNN
	1    4300 5900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4300 5750 4300 5700
Connection ~ 4300 5700
Wire Wire Line
	5050 6100 4300 6100
Wire Wire Line
	4300 6100 4300 6050
Connection ~ 5050 6100
Text Label 5550 5300 0    60   ~ 0
OLED_GND
Connection ~ 4800 5300
Wire Wire Line
	4500 5300 4500 4900
Wire Notes Line
	4200 5400 4200 6150
Wire Notes Line
	4200 6150 5600 6150
Wire Notes Line
	5600 6150 5600 5400
Wire Notes Line
	5600 5400 4200 5400
Text Notes 5700 5800 0    60   ~ 0
Power gating for entire display\nto reduce pesky leakage
Text Notes 5700 6150 0    60   ~ 0
Rds should be around <2R for each transistor while under 200mA
$EndSCHEMATC
