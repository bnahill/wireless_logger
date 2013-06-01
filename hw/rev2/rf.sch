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
Sheet 4 7
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
L CC1101 U9
U 1 1 5194854C
P 5850 3700
F 0 "U9" H 6200 3000 60  0000 C CNN
F 1 "CC1101" H 6250 4350 60  0000 C CNN
F 2 "~" H 5850 3700 60  0000 C CNN
F 3 "~" H 5850 3700 60  0000 C CNN
	1    5850 3700
	1    0    0    -1  
$EndComp
$Comp
L JOHAN_BALUN_FILT B1
U 1 1 519485BA
P 8150 3800
F 0 "B1" H 8250 3600 60  0000 L CNN
F 1 "0915BM15A0001" H 8150 4000 60  0000 C CNN
F 2 "~" H 8000 3750 60  0000 C CNN
F 3 "~" H 8000 3750 60  0000 C CNN
	1    8150 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3800 6550 3800
Wire Wire Line
	6550 3700 7850 3700
Text Notes 8650 3850 0    60   ~ 0
0433BM15A0001 for 433MHz
Wire Wire Line
	4850 3800 5150 3800
$Comp
L CAP C23
U 1 1 519489C7
P 4950 4100
F 0 "C23" H 4970 4070 50  0000 L TNN
F 1 "100n" H 4975 4120 30  0000 L BNN
F 2 "" H 4950 4100 60  0001 C CNN
F 3 "" H 4950 4100 60  0001 C CNN
	1    4950 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 4000 4950 3900
Wire Wire Line
	4950 3900 5150 3900
$Comp
L GND #PWR029
U 1 1 519489E8
P 4950 4250
F 0 "#PWR029" H 4950 4250 30  0001 C CNN
F 1 "GND" H 4950 4180 30  0001 C CNN
F 2 "" H 4950 4250 60  0000 C CNN
F 3 "" H 4950 4250 60  0000 C CNN
	1    4950 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4250 4950 4200
Text HLabel 5750 4650 3    60   Input ~ 0
nCS
Wire Wire Line
	5750 4400 5750 4650
$Comp
L GND #PWR030
U 1 1 51948A2D
P 5950 5250
F 0 "#PWR030" H 5950 5250 30  0001 C CNN
F 1 "GND" H 5950 5180 30  0001 C CNN
F 2 "" H 5950 5250 60  0000 C CNN
F 3 "" H 5950 5250 60  0000 C CNN
	1    5950 5250
	1    0    0    -1  
$EndComp
$Comp
L CAP C25
U 1 1 51948A3C
P 6050 5050
F 0 "C25" H 6070 5020 50  0000 L TNN
F 1 "5p" H 6075 5070 30  0000 L BNN
F 2 "~" H 6050 5050 60  0000 C CNN
F 3 "~" H 6050 5050 60  0000 C CNN
	1    6050 5050
	0    -1   -1   0   
$EndComp
$Comp
L CAP C24
U 1 1 51948A49
P 5850 5050
F 0 "C24" H 5870 5020 50  0000 L TNN
F 1 "5p" H 5875 5070 30  0000 L BNN
F 2 "~" H 5850 5050 60  0000 C CNN
F 3 "~" H 5850 5050 60  0000 C CNN
	1    5850 5050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5950 5250 5950 5200
Wire Wire Line
	5850 5200 6050 5200
Wire Wire Line
	6050 5200 6050 5150
Wire Wire Line
	5850 5200 5850 5150
Connection ~ 5950 5200
Wire Wire Line
	5850 4400 5850 4950
Connection ~ 5850 4800
Wire Wire Line
	6050 4400 6050 4950
Connection ~ 6050 4800
Wire Wire Line
	5950 4450 5950 4400
Wire Wire Line
	6650 3900 6550 3900
$Comp
L CAP C26
U 1 1 51948B7B
P 7000 3450
F 0 "C26" H 7020 3420 50  0000 L TNN
F 1 "10n" H 7025 3470 30  0000 L BNN
F 2 "SMD0402" H 7000 3450 60  0001 C CNN
F 3 "~" H 7000 3450 60  0000 C CNN
	1    7000 3450
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR031
U 1 1 51948B90
P 7000 3600
F 0 "#PWR031" H 7000 3600 30  0001 C CNN
F 1 "GND" H 7000 3530 30  0001 C CNN
F 2 "" H 7000 3600 60  0000 C CNN
F 3 "" H 7000 3600 60  0000 C CNN
	1    7000 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3600 7000 3550
Wire Wire Line
	6850 3600 6550 3600
Wire Wire Line
	6850 3150 6850 3600
Wire Wire Line
	6550 3500 6850 3500
Connection ~ 6850 3500
Text HLabel 4750 3500 0    60   Input ~ 0
SCLK
$Comp
L RES R13
U 1 1 51948C4F
P 5600 4850
F 0 "R13" H 5670 4815 50  0000 L TNN
F 1 "220k" H 5600 4905 30  0000 C BNN
F 2 "~" H 5600 4850 60  0000 C CNN
F 3 "~" H 5600 4850 60  0000 C CNN
	1    5600 4850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5600 4700 5600 4600
Wire Wire Line
	5600 4600 5750 4600
Connection ~ 5750 4600
Wire Wire Line
	5600 5150 5600 5000
Text HLabel 4750 3600 0    60   Output ~ 0
SDO
Wire Wire Line
	4750 3600 5150 3600
Wire Wire Line
	5150 3500 4750 3500
Text HLabel 5650 2750 1    60   Input ~ 0
SDI
Wire Wire Line
	5650 2750 5650 3000
$Comp
L GND #PWR032
U 1 1 51948D5E
P 5450 2650
F 0 "#PWR032" H 5450 2650 30  0001 C CNN
F 1 "GND" H 5450 2580 30  0001 C CNN
F 2 "" H 5450 2650 60  0000 C CNN
F 3 "" H 5450 2650 60  0000 C CNN
	1    5450 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2650 5450 2450
Wire Wire Line
	5450 2450 5750 2450
Wire Wire Line
	5750 2450 5750 3000
$Comp
L RES R14
U 1 1 51948E80
P 6050 2700
F 0 "R14" H 6120 2665 50  0000 L TNN
F 1 "56k (1%)" H 6050 2755 30  0000 C BNN
F 2 "~" H 6050 2700 60  0000 C CNN
F 3 "~" H 6050 2700 60  0000 C CNN
	1    6050 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6050 2850 6050 3000
Wire Wire Line
	5950 3000 5950 2450
Wire Wire Line
	5950 2450 6050 2450
Wire Wire Line
	6050 2450 6050 2550
$Comp
L GND #PWR033
U 1 1 51948F03
P 6550 3000
F 0 "#PWR033" H 6550 3000 30  0001 C CNN
F 1 "GND" H 6550 2930 30  0001 C CNN
F 2 "" H 6550 3000 60  0000 C CNN
F 3 "" H 6550 3000 60  0000 C CNN
	1    6550 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3000 6550 2900
Wire Wire Line
	6550 2900 6050 2900
Connection ~ 6050 2900
Wire Wire Line
	5850 2750 5850 3000
$Comp
L RES R15
U 1 1 51948F8F
P 6850 3000
F 0 "R15" H 6920 2965 50  0000 L TNN
F 1 "4R7" H 6850 3055 30  0000 C BNN
F 2 "~" H 6850 3000 60  0000 C CNN
F 3 "~" H 6850 3000 60  0000 C CNN
	1    6850 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6850 2750 6850 2850
Wire Wire Line
	6850 3250 7000 3250
Wire Wire Line
	7000 3250 7000 3350
Connection ~ 6850 3250
Text HLabel 4750 3700 0    60   BiDi ~ 0
GDO2
Wire Wire Line
	4750 3700 5150 3700
Text HLabel 5400 4500 0    60   BiDi ~ 0
GDO0
Wire Wire Line
	5400 4500 5650 4500
Wire Wire Line
	5650 4500 5650 4400
$Comp
L ANTENNA ANT1
U 1 1 51949426
P 9250 3250
F 0 "ANT1" H 9500 3050 60  0000 C CNN
F 1 "ANTENNA" H 9500 3450 60  0000 C CNN
F 2 "~" H 9250 3250 60  0000 C CNN
F 3 "~" H 9250 3250 60  0000 C CNN
	1    9250 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 3700 9250 3700
Wire Wire Line
	9250 3700 9250 3400
$Comp
L GND #PWR034
U 1 1 51949466
P 8550 4200
F 0 "#PWR034" H 8550 4200 30  0001 C CNN
F 1 "GND" H 8550 4130 30  0001 C CNN
F 2 "" H 8550 4200 60  0000 C CNN
F 3 "" H 8550 4200 60  0000 C CNN
	1    8550 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3800 8550 4200
Wire Wire Line
	8550 3900 8450 3900
Wire Wire Line
	8450 3800 8550 3800
Connection ~ 8550 3900
Wire Wire Line
	8550 4100 7750 4100
Wire Wire Line
	7750 4100 7750 3900
Wire Wire Line
	7750 3900 7850 3900
Connection ~ 8550 4100
Text GLabel 5850 2750 1    60   Input ~ 0
V3
Text GLabel 6850 2750 1    60   Input ~ 0
V3
Text GLabel 6650 3900 2    60   Input ~ 0
V3
Text GLabel 5950 4450 3    60   Input ~ 0
V3
Text GLabel 5600 5150 3    60   Input ~ 0
V3
Text GLabel 4850 3800 0    60   Input ~ 0
V3
$Comp
L XTAL X1
U 1 1 51971962
P 5950 4800
F 0 "X1" H 5950 4740 40  0000 C TNN
F 1 "FA-128 26.0000MF10Z-W3" H 6500 4750 40  0000 C BNN
F 2 "~" H 5950 4800 60  0000 C CNN
F 3 "~" H 5950 4800 60  0000 C CNN
	1    5950 4800
	1    0    0    -1  
$EndComp
Text Label 9250 3700 0    60   ~ 0
GUARDIAN_ANT_SE
$EndSCHEMATC
