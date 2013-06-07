EESchema Schematic File Version 2  date Thu 02 May 2013 07:35:28 PM CEST
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
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:main-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "2 may 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 2300 3600
Wire Wire Line
	2750 3400 2650 3400
Wire Wire Line
	2650 3400 2650 3600
Wire Wire Line
	2650 3600 1950 3600
Connection ~ 2300 3000
Wire Wire Line
	1950 3000 2650 3000
Wire Wire Line
	4650 4300 4900 4300
Wire Wire Line
	4650 4100 4900 4100
Wire Wire Line
	4550 1400 5250 1400
Wire Wire Line
	4650 3600 4900 3600
Connection ~ 6750 1050
Wire Wire Line
	6750 1050 6750 1500
Wire Wire Line
	6750 1500 6850 1500
Wire Wire Line
	6600 1300 6600 1050
Wire Wire Line
	6600 1050 6850 1050
Wire Wire Line
	6900 700  7000 700 
Connection ~ 6300 950 
Wire Wire Line
	6300 950  6300 700 
Wire Wire Line
	6300 700  6400 700 
Wire Wire Line
	800  1450 1150 1450
Wire Wire Line
	1550 1100 1700 1100
Wire Wire Line
	2200 1450 2350 1450
Wire Wire Line
	2200 750  2350 750 
Wire Wire Line
	800  750  1150 750 
Wire Wire Line
	6200 1800 6200 1950
Connection ~ 6450 950 
Wire Wire Line
	6050 850  6050 950 
Wire Wire Line
	6050 950  6850 950 
Wire Wire Line
	6450 950  6450 1400
Wire Wire Line
	5050 600  5050 750 
Connection ~ 5050 1400
Wire Wire Line
	5050 1250 5050 1400
Wire Wire Line
	4650 3000 4900 3000
Wire Wire Line
	4650 2800 4900 2800
Wire Wire Line
	1250 5350 1600 5350
Wire Wire Line
	1250 5150 1600 5150
Wire Wire Line
	2400 5250 2700 5250
Wire Wire Line
	1400 3900 1400 3600
Wire Wire Line
	1400 3600 1550 3600
Wire Wire Line
	3650 5100 3650 5350
Wire Wire Line
	3950 2150 3950 2250
Connection ~ 3650 1750
Wire Wire Line
	3650 2200 3650 1500
Wire Wire Line
	4350 1750 3650 1750
Connection ~ 3950 1750
Wire Wire Line
	4350 2150 4350 2250
Wire Wire Line
	2550 1850 2550 2000
Wire Wire Line
	1550 3000 1400 3000
Wire Wire Line
	1400 3000 1400 3250
Wire Wire Line
	2400 5150 2700 5150
Wire Wire Line
	2700 5350 2400 5350
Wire Wire Line
	1250 5250 1600 5250
Wire Wire Line
	2750 2500 2200 2500
Connection ~ 2550 2500
Wire Wire Line
	4650 2900 4900 2900
Wire Wire Line
	4850 1600 4850 1250
Connection ~ 4850 1600
Wire Wire Line
	4850 600  4850 750 
Wire Wire Line
	6200 1150 6200 1400
Wire Wire Line
	5950 850  5950 1150
Connection ~ 6200 1150
Wire Wire Line
	6450 1800 6450 1950
Wire Wire Line
	1700 750  1550 750 
Wire Wire Line
	2200 1100 2350 1100
Wire Wire Line
	1550 1450 1700 1450
Wire Wire Line
	800  1100 1150 1100
Wire Wire Line
	7400 700  7500 700 
Wire Wire Line
	7500 700  7500 850 
Wire Wire Line
	5950 1150 6850 1150
Wire Wire Line
	6850 1400 6800 1400
Wire Wire Line
	6800 1400 6800 950 
Connection ~ 6800 950 
Wire Wire Line
	6850 1600 6700 1600
Wire Wire Line
	6700 1600 6700 1150
Connection ~ 6700 1150
Wire Wire Line
	4650 3700 4900 3700
Wire Wire Line
	4550 1600 4950 1600
Wire Wire Line
	4950 1600 4950 1500
Wire Wire Line
	4950 1500 5250 1500
Wire Wire Line
	5250 1600 5150 1600
Wire Wire Line
	5150 1600 5150 1750
Wire Wire Line
	4650 4200 4900 4200
Wire Wire Line
	2750 3200 2650 3200
Wire Wire Line
	2650 3200 2650 3000
$Comp
L CRYSTAL X1
U 1 1 51814C79
P 2300 3300
F 0 "X1" H 2300 3450 60  0000 C CNN
F 1 "12MHz" H 2300 3150 60  0000 C CNN
F 2 "" H 2300 3300 60  0001 C CNN
F 3 "" H 2300 3300 60  0001 C CNN
	1    2300 3300
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR01
U 1 1 51814B83
P 5150 1750
F 0 "#PWR01" H 5150 1750 30  0001 C CNN
F 1 "GND" H 5150 1680 30  0001 C CNN
F 2 "" H 5150 1750 60  0001 C CNN
F 3 "" H 5150 1750 60  0001 C CNN
	1    5150 1750
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 K11
U 1 1 51814B69
P 5600 1500
F 0 "K11" V 5550 1500 50  0000 C CNN
F 1 "I2C" V 5650 1500 40  0000 C CNN
F 2 "" H 5600 1500 60  0001 C CNN
F 3 "" H 5600 1500 60  0001 C CNN
	1    5600 1500
	1    0    0    -1  
$EndComp
Text Label 4900 3700 2    60   ~ 0
SCL
Text Label 4900 3600 2    60   ~ 0
SDA
$Comp
L CONN_3 K10
U 1 1 5180E021
P 7200 1500
F 0 "K10" V 7150 1500 50  0000 C CNN
F 1 "CPOW2" V 7250 1500 40  0000 C CNN
F 2 "" H 7200 1500 60  0001 C CNN
F 3 "" H 7200 1500 60  0001 C CNN
	1    7200 1500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 51803CA2
P 7500 850
F 0 "#PWR03" H 7500 850 30  0001 C CNN
F 1 "GND" H 7500 780 30  0001 C CNN
F 2 "" H 7500 850 60  0001 C CNN
F 3 "" H 7500 850 60  0001 C CNN
	1    7500 850 
	1    0    0    -1  
$EndComp
$Comp
L LED D5
U 1 1 51803C7F
P 7200 700
F 0 "D5" H 7200 800 50  0000 C CNN
F 1 "LED" H 7200 600 50  0000 C CNN
F 2 "" H 7200 700 60  0001 C CNN
F 3 "" H 7200 700 60  0001 C CNN
	1    7200 700 
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 51803C7E
P 6650 700
F 0 "R7" V 6730 700 50  0000 C CNN
F 1 "150" V 6650 700 50  0000 C CNN
F 2 "" H 6650 700 60  0001 C CNN
F 3 "" H 6650 700 60  0001 C CNN
	1    6650 700 
	0    1    1    0   
$EndComp
Text Label 4900 4300 2    60   ~ 0
LED3
Text Label 4900 4200 2    60   ~ 0
LED2
Text Label 4900 4100 2    60   ~ 0
LED1
Text Label 800  1450 0    60   ~ 0
LED3
Text Label 800  1100 0    60   ~ 0
LED2
Text Label 800  750  0    60   ~ 0
LED1
$Comp
L VCC #PWR012
U 1 1 51803AA7
P 2350 1450
F 0 "#PWR012" H 2350 1550 30  0001 C CNN
F 1 "VCC" H 2350 1550 30  0000 C CNN
F 2 "" H 2350 1450 60  0001 C CNN
F 3 "" H 2350 1450 60  0001 C CNN
	1    2350 1450
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR013
U 1 1 51803AA6
P 2350 1100
F 0 "#PWR013" H 2350 1200 30  0001 C CNN
F 1 "VCC" H 2350 1200 30  0000 C CNN
F 2 "" H 2350 1100 60  0001 C CNN
F 3 "" H 2350 1100 60  0001 C CNN
	1    2350 1100
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR014
U 1 1 51803AA3
P 2350 750
F 0 "#PWR014" H 2350 850 30  0001 C CNN
F 1 "VCC" H 2350 850 30  0000 C CNN
F 2 "" H 2350 750 60  0001 C CNN
F 3 "" H 2350 750 60  0001 C CNN
	1    2350 750 
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 51803A8A
P 1950 1450
F 0 "R3" V 2030 1450 50  0000 C CNN
F 1 "150" V 1950 1450 50  0000 C CNN
F 2 "" H 1950 1450 60  0001 C CNN
F 3 "" H 1950 1450 60  0001 C CNN
	1    1950 1450
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 51803A84
P 1950 1100
F 0 "R2" V 2030 1100 50  0000 C CNN
F 1 "150" V 1950 1100 50  0000 C CNN
F 2 "" H 1950 1100 60  0001 C CNN
F 3 "" H 1950 1100 60  0001 C CNN
	1    1950 1100
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 51803A7F
P 1950 750
F 0 "R1" V 2030 750 50  0000 C CNN
F 1 "150" V 1950 750 50  0000 C CNN
F 2 "" H 1950 750 60  0001 C CNN
F 3 "" H 1950 750 60  0001 C CNN
	1    1950 750 
	0    1    1    0   
$EndComp
$Comp
L LED D3
U 1 1 51803A76
P 1350 1450
F 0 "D3" H 1350 1550 50  0000 C CNN
F 1 "LED" H 1350 1350 50  0000 C CNN
F 2 "" H 1350 1450 60  0001 C CNN
F 3 "" H 1350 1450 60  0001 C CNN
	1    1350 1450
	-1   0    0    1   
$EndComp
$Comp
L LED D2
U 1 1 51803A71
P 1350 1100
F 0 "D2" H 1350 1200 50  0000 C CNN
F 1 "LED" H 1350 1000 50  0000 C CNN
F 2 "" H 1350 1100 60  0001 C CNN
F 3 "" H 1350 1100 60  0001 C CNN
	1    1350 1100
	-1   0    0    1   
$EndComp
$Comp
L LED D1
U 1 1 51803A62
P 1350 750
F 0 "D1" H 1350 850 50  0000 C CNN
F 1 "LED" H 1350 650 50  0000 C CNN
F 2 "" H 1350 750 60  0001 C CNN
F 3 "" H 1350 750 60  0001 C CNN
	1    1350 750 
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR023
U 1 1 51803455
P 6200 1950
F 0 "#PWR023" H 6200 1950 30  0001 C CNN
F 1 "GND" H 6200 1880 30  0001 C CNN
F 2 "" H 6200 1950 60  0001 C CNN
F 3 "" H 6200 1950 60  0001 C CNN
	1    6200 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 51803453
P 6450 1950
F 0 "#PWR024" H 6450 1950 30  0001 C CNN
F 1 "GND" H 6450 1880 30  0001 C CNN
F 2 "" H 6450 1950 60  0001 C CNN
F 3 "" H 6450 1950 60  0001 C CNN
	1    6450 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 5180342A
P 6600 1300
F 0 "#PWR025" H 6600 1300 30  0001 C CNN
F 1 "GND" H 6600 1230 30  0001 C CNN
F 2 "" H 6600 1300 60  0001 C CNN
F 3 "" H 6600 1300 60  0001 C CNN
	1    6600 1300
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 51803412
P 6200 1600
F 0 "C5" H 6250 1700 50  0000 L CNN
F 1 "100nF" H 6250 1500 50  0000 L CNN
F 2 "" H 6200 1600 60  0001 C CNN
F 3 "" H 6200 1600 60  0001 C CNN
	1    6200 1600
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 5180340B
P 6450 1600
F 0 "C10" H 6500 1700 50  0000 L CNN
F 1 "100nF" H 6500 1500 50  0000 L CNN
F 2 "" H 6450 1600 60  0001 C CNN
F 3 "" H 6450 1600 60  0001 C CNN
	1    6450 1600
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR026
U 1 1 518033D3
P 5950 850
F 0 "#PWR026" H 5950 950 30  0001 C CNN
F 1 "VDD" H 5950 960 30  0000 C CNN
F 2 "" H 5950 850 60  0001 C CNN
F 3 "" H 5950 850 60  0001 C CNN
	1    5950 850 
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 K9
U 1 1 518033B4
P 7200 1050
F 0 "K9" V 7150 1050 50  0000 C CNN
F 1 "CPOW" V 7250 1050 40  0000 C CNN
F 2 "" H 7200 1050 60  0001 C CNN
F 3 "" H 7200 1050 60  0001 C CNN
	1    7200 1050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR027
U 1 1 51803382
P 6050 850
F 0 "#PWR027" H 6050 950 30  0001 C CNN
F 1 "VCC" H 6050 950 30  0000 C CNN
F 2 "" H 6050 850 60  0001 C CNN
F 3 "" H 6050 850 60  0001 C CNN
	1    6050 850 
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR028
U 1 1 51803343
P 5050 600
F 0 "#PWR028" H 5050 700 30  0001 C CNN
F 1 "VCC" H 5050 700 30  0000 C CNN
F 2 "" H 5050 600 60  0001 C CNN
F 3 "" H 5050 600 60  0001 C CNN
	1    5050 600 
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR029
U 1 1 51803340
P 4850 600
F 0 "#PWR029" H 4850 700 30  0001 C CNN
F 1 "VCC" H 4850 700 30  0000 C CNN
F 2 "" H 4850 600 60  0001 C CNN
F 3 "" H 4850 600 60  0001 C CNN
	1    4850 600 
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 51803330
P 4850 1000
F 0 "R5" V 4930 1000 50  0000 C CNN
F 1 "2k2" V 4850 1000 50  0000 C CNN
F 2 "" H 4850 1000 60  0001 C CNN
F 3 "" H 4850 1000 60  0001 C CNN
	1    4850 1000
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5180332A
P 5050 1000
F 0 "R6" V 5130 1000 50  0000 C CNN
F 1 "2k2" V 5050 1000 50  0000 C CNN
F 2 "" H 5050 1000 60  0001 C CNN
F 3 "" H 5050 1000 60  0001 C CNN
	1    5050 1000
	1    0    0    -1  
$EndComp
Text Label 4550 1600 0    60   ~ 0
SCL
Text Label 4550 1400 0    60   ~ 0
SDA
Text Label 2200 2500 0    60   ~ 0
RESET
Text Label 4900 2800 2    60   ~ 0
MOSI
Text Label 4900 2900 2    60   ~ 0
MISO
Text Label 4900 3000 2    60   ~ 0
SCK
Text Label 2700 5350 2    60   ~ 0
RESET
$Comp
L VCC #PWR030
U 1 1 51803005
P 2700 5250
F 0 "#PWR030" H 2700 5350 30  0001 C CNN
F 1 "VCC" H 2700 5350 30  0000 C CNN
F 2 "" H 2700 5250 60  0001 C CNN
F 3 "" H 2700 5250 60  0001 C CNN
	1    2700 5250
	0    1    1    0   
$EndComp
$Comp
L GND #PWR031
U 1 1 51802FFD
P 2700 5150
F 0 "#PWR031" H 2700 5150 30  0001 C CNN
F 1 "GND" H 2700 5080 30  0001 C CNN
F 2 "" H 2700 5150 60  0001 C CNN
F 3 "" H 2700 5150 60  0001 C CNN
	1    2700 5150
	0    -1   -1   0   
$EndComp
Text Label 1250 5350 0    60   ~ 0
SCK
Text Label 1250 5250 0    60   ~ 0
MISO
Text Label 1250 5150 0    60   ~ 0
MOSI
$Comp
L CONN_3X2 P1
U 1 1 51802F91
P 2000 5300
F 0 "P1" H 2000 5550 50  0000 C CNN
F 1 "PROG" V 2000 5350 40  0000 C CNN
F 2 "" H 2000 5300 60  0001 C CNN
F 3 "" H 2000 5300 60  0001 C CNN
	1    2000 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 51802A6E
P 1400 3900
F 0 "#PWR032" H 1400 3900 30  0001 C CNN
F 1 "GND" H 1400 3830 30  0001 C CNN
F 2 "" H 1400 3900 60  0001 C CNN
F 3 "" H 1400 3900 60  0001 C CNN
	1    1400 3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 51802A6B
P 1400 3250
F 0 "#PWR033" H 1400 3250 30  0001 C CNN
F 1 "GND" H 1400 3180 30  0001 C CNN
F 2 "" H 1400 3250 60  0001 C CNN
F 3 "" H 1400 3250 60  0001 C CNN
	1    1400 3250
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 51802A62
P 1750 3600
F 0 "C2" H 1800 3700 50  0000 L CNN
F 1 "22pF" H 1800 3500 50  0000 L CNN
F 2 "" H 1750 3600 60  0001 C CNN
F 3 "" H 1750 3600 60  0001 C CNN
	1    1750 3600
	0    -1   -1   0   
$EndComp
$Comp
L C C1
U 1 1 51802A5D
P 1750 3000
F 0 "C1" H 1800 3100 50  0000 L CNN
F 1 "22pF" H 1800 2900 50  0000 L CNN
F 2 "" H 1750 3000 60  0001 C CNN
F 3 "" H 1750 3000 60  0001 C CNN
	1    1750 3000
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR034
U 1 1 51802A11
P 2550 1850
F 0 "#PWR034" H 2550 1950 30  0001 C CNN
F 1 "VCC" H 2550 1950 30  0000 C CNN
F 2 "" H 2550 1850 60  0001 C CNN
F 3 "" H 2550 1850 60  0001 C CNN
	1    2550 1850
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR035
U 1 1 518029F2
P 3650 1500
F 0 "#PWR035" H 3650 1600 30  0001 C CNN
F 1 "VCC" H 3650 1600 30  0000 C CNN
F 2 "" H 3650 1500 60  0001 C CNN
F 3 "" H 3650 1500 60  0001 C CNN
	1    3650 1500
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 518029CD
P 4350 1950
F 0 "C4" H 4400 2050 50  0000 L CNN
F 1 "10uF" H 4400 1850 50  0000 L CNN
F 2 "" H 4350 1950 60  0001 C CNN
F 3 "" H 4350 1950 60  0001 C CNN
	1    4350 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR036
U 1 1 518029C4
P 4350 2250
F 0 "#PWR036" H 4350 2250 30  0001 C CNN
F 1 "GND" H 4350 2180 30  0001 C CNN
F 2 "" H 4350 2250 60  0001 C CNN
F 3 "" H 4350 2250 60  0001 C CNN
	1    4350 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR037
U 1 1 518029BD
P 3950 2250
F 0 "#PWR037" H 3950 2250 30  0001 C CNN
F 1 "GND" H 3950 2180 30  0001 C CNN
F 2 "" H 3950 2250 60  0001 C CNN
F 3 "" H 3950 2250 60  0001 C CNN
	1    3950 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR038
U 1 1 518029B7
P 3650 5350
F 0 "#PWR038" H 3650 5350 30  0001 C CNN
F 1 "GND" H 3650 5280 30  0001 C CNN
F 2 "" H 3650 5350 60  0001 C CNN
F 3 "" H 3650 5350 60  0001 C CNN
	1    3650 5350
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 518029A0
P 3950 1950
F 0 "C3" H 4000 2050 50  0000 L CNN
F 1 "100nF" H 4000 1850 50  0000 L CNN
F 2 "" H 3950 1950 60  0001 C CNN
F 3 "" H 3950 1950 60  0001 C CNN
	1    3950 1950
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 518028F7
P 2550 2250
F 0 "R4" V 2630 2250 50  0000 C CNN
F 1 "10k" V 2550 2250 50  0000 C CNN
F 2 "" H 2550 2250 60  0001 C CNN
F 3 "" H 2550 2250 60  0001 C CNN
	1    2550 2250
	1    0    0    -1  
$EndComp
$Comp
L ATMEGA8-P IC1
U 1 1 518028E2
P 3650 3600
F 0 "IC1" H 2950 4850 50  0000 L BNN
F 1 "ATMEGA8-P" H 4000 2200 50  0000 L BNN
F 2 "DIL28" H 3050 2250 50  0001 C CNN
F 3 "" H 3650 3600 60  0001 C CNN
	1    3650 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC