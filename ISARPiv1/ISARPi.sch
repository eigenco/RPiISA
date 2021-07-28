EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Bus_ISA_8bit J1
U 1 1 60405B30
P 1800 2525
F 0 "J1" H 1800 4292 50  0000 C CNN
F 1 "Bus_ISA_8bit" H 1800 4201 50  0000 C CNN
F 2 "adlib:BUS_PC" H 1800 2525 50  0001 C CNN
F 3 "https://en.wikipedia.org/wiki/Industry_Standard_Architecture" H 1800 2525 50  0001 C CNN
	1    1800 2525
	1    0    0    -1  
$EndComp
Text GLabel 2500 3125 2    50   BiDi ~ 0
A9
Text GLabel 2500 3225 2    50   BiDi ~ 0
A8
Text GLabel 2500 3325 2    50   BiDi ~ 0
A7
Text GLabel 2500 3425 2    50   BiDi ~ 0
A6
Text GLabel 2500 3525 2    50   BiDi ~ 0
A5
Text GLabel 2500 3625 2    50   BiDi ~ 0
A4
Text GLabel 2500 3725 2    50   BiDi ~ 0
A3
Text GLabel 2500 3825 2    50   BiDi ~ 0
A2
Text GLabel 2500 3925 2    50   BiDi ~ 0
A1
Text GLabel 2500 4025 2    50   BiDi ~ 0
A0
Text GLabel 1100 1225 0    50   BiDi ~ 0
VCC
Text GLabel 1100 2225 0    50   BiDi ~ 0
nIOW
Text GLabel 1100 2325 0    50   BiDi ~ 0
nIOR
$Comp
L Connector:Raspberry_Pi_2_3 J2
U 1 1 60DDB778
P 4825 2550
F 0 "J2" H 4825 4031 50  0000 C CNN
F 1 "Raspberry_Pi_2_3" H 4825 3940 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x20_P2.54mm_Vertical" H 4825 2550 50  0001 C CNN
F 3 "https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/rpi_SCH_3bplus_1p0_reduced.pdf" H 4825 2550 50  0001 C CNN
	1    4825 2550
	1    0    0    -1  
$EndComp
Text GLabel 4425 3850 3    50   BiDi ~ 0
GND
Text GLabel 5625 3250 2    50   BiDi ~ 0
RA0
Text GLabel 5625 3350 2    50   BiDi ~ 0
RA1
Text GLabel 4025 1650 0    50   BiDi ~ 0
RA2
Text GLabel 4025 1750 0    50   BiDi ~ 0
RA3
Text GLabel 4025 1950 0    50   BiDi ~ 0
RA4
Text GLabel 4025 2050 0    50   BiDi ~ 0
RA5
Text GLabel 4025 2150 0    50   BiDi ~ 0
RA6
Text GLabel 4025 2350 0    50   BiDi ~ 0
RA7
Text GLabel 4025 2450 0    50   BiDi ~ 0
RA8
Text GLabel 4025 2550 0    50   BiDi ~ 0
RA9
Text GLabel 2500 1825 2    50   BiDi ~ 0
D0
Text GLabel 2500 1725 2    50   BiDi ~ 0
D1
Text GLabel 2500 1625 2    50   BiDi ~ 0
D2
Text GLabel 2500 1525 2    50   BiDi ~ 0
D3
Text GLabel 2500 1425 2    50   BiDi ~ 0
D4
Text GLabel 2500 1325 2    50   BiDi ~ 0
D5
Text GLabel 2500 1225 2    50   BiDi ~ 0
D6
Text GLabel 2500 1125 2    50   BiDi ~ 0
D7
Text GLabel 4525 3850 3    50   BiDi ~ 0
GND
Text GLabel 4625 3850 3    50   BiDi ~ 0
GND
Text GLabel 4725 3850 3    50   BiDi ~ 0
GND
Text GLabel 4825 3850 3    50   BiDi ~ 0
GND
Text GLabel 4925 3850 3    50   BiDi ~ 0
GND
Text GLabel 5025 3850 3    50   BiDi ~ 0
GND
Text GLabel 5125 3850 3    50   BiDi ~ 0
GND
Text GLabel 1100 1025 0    50   BiDi ~ 0
GND
Text GLabel 4625 1250 1    50   BiDi ~ 0
VCC
Text GLabel 4725 1250 1    50   BiDi ~ 0
VCC
Text GLabel 1100 1925 0    50   BiDi ~ 0
GND
Text GLabel 1100 3825 0    50   BiDi ~ 0
VCC
Text GLabel 1100 4025 0    50   BiDi ~ 0
GND
$Comp
L Device:R R0
U 1 1 60DE8F33
P 3500 4775
F 0 "R0" V 3400 4775 50  0000 C CNN
F 1 "100" V 3500 4775 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 4775 50  0001 C CNN
F 3 "~" H 3500 4775 50  0001 C CNN
	1    3500 4775
	0    1    1    0   
$EndComp
Text GLabel 3350 4775 0    50   BiDi ~ 0
A0
Text GLabel 3650 4775 2    50   BiDi ~ 0
RA0
$Comp
L Device:R R1
U 1 1 60DE9BCA
P 3500 5075
F 0 "R1" V 3400 5075 50  0000 C CNN
F 1 "100" V 3500 5075 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 5075 50  0001 C CNN
F 3 "~" H 3500 5075 50  0001 C CNN
	1    3500 5075
	0    1    1    0   
$EndComp
Text GLabel 3650 5075 2    50   BiDi ~ 0
RA1
Text GLabel 3350 5075 0    50   BiDi ~ 0
A1
$Comp
L Device:R R2
U 1 1 60DE9F23
P 3500 5375
F 0 "R2" V 3400 5375 50  0000 C CNN
F 1 "100" V 3500 5375 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 5375 50  0001 C CNN
F 3 "~" H 3500 5375 50  0001 C CNN
	1    3500 5375
	0    1    1    0   
$EndComp
Text GLabel 3350 5375 0    50   BiDi ~ 0
A2
Text GLabel 3650 5375 2    50   BiDi ~ 0
RA2
$Comp
L Device:R R3
U 1 1 60DEA222
P 3500 5675
F 0 "R3" V 3400 5675 50  0000 C CNN
F 1 "100" V 3500 5675 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 5675 50  0001 C CNN
F 3 "~" H 3500 5675 50  0001 C CNN
	1    3500 5675
	0    1    1    0   
$EndComp
Text GLabel 3350 5675 0    50   BiDi ~ 0
A3
Text GLabel 3650 5675 2    50   BiDi ~ 0
RA3
$Comp
L Device:R R4
U 1 1 60DEA549
P 3500 5975
F 0 "R4" V 3400 5975 50  0000 C CNN
F 1 "100" V 3500 5975 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 5975 50  0001 C CNN
F 3 "~" H 3500 5975 50  0001 C CNN
	1    3500 5975
	0    1    1    0   
$EndComp
Text GLabel 3350 5975 0    50   BiDi ~ 0
A4
Text GLabel 3650 5975 2    50   BiDi ~ 0
RA4
$Comp
L Device:R R5
U 1 1 60DEA87A
P 3500 6275
F 0 "R5" V 3293 6275 50  0000 C CNN
F 1 "100" V 3500 6275 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 6275 50  0001 C CNN
F 3 "~" H 3500 6275 50  0001 C CNN
	1    3500 6275
	0    1    1    0   
$EndComp
Text GLabel 3350 6275 0    50   BiDi ~ 0
A5
Text GLabel 3650 6275 2    50   BiDi ~ 0
RA5
$Comp
L Device:R R6
U 1 1 60DEADE0
P 3500 6575
F 0 "R6" V 3400 6575 50  0000 C CNN
F 1 "100" V 3500 6575 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 6575 50  0001 C CNN
F 3 "~" H 3500 6575 50  0001 C CNN
	1    3500 6575
	0    1    1    0   
$EndComp
Text GLabel 3350 6575 0    50   BiDi ~ 0
A6
Text GLabel 3650 6575 2    50   BiDi ~ 0
RA6
$Comp
L Device:R R7
U 1 1 60DEB14A
P 3500 6875
F 0 "R7" V 3400 6875 50  0000 C CNN
F 1 "100" V 3500 6875 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 6875 50  0001 C CNN
F 3 "~" H 3500 6875 50  0001 C CNN
	1    3500 6875
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 60DEB3D8
P 3500 7175
F 0 "R8" V 3400 7175 50  0000 C CNN
F 1 "100" V 3500 7175 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 7175 50  0001 C CNN
F 3 "~" H 3500 7175 50  0001 C CNN
	1    3500 7175
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 60DEB5D8
P 3500 7475
F 0 "R9" V 3400 7475 50  0000 C CNN
F 1 "100" V 3500 7475 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 7475 50  0001 C CNN
F 3 "~" H 3500 7475 50  0001 C CNN
	1    3500 7475
	0    1    1    0   
$EndComp
Text GLabel 3350 6875 0    50   BiDi ~ 0
A7
Text GLabel 3350 7175 0    50   BiDi ~ 0
A8
Text GLabel 3350 7475 0    50   BiDi ~ 0
A9
Text GLabel 3650 6875 2    50   BiDi ~ 0
RA7
Text GLabel 3650 7175 2    50   BiDi ~ 0
RA8
Text GLabel 3650 7475 2    50   BiDi ~ 0
RA9
Text GLabel 4125 4775 0    50   BiDi ~ 0
D0
$Comp
L Device:R R10
U 1 1 60DEBC37
P 4275 4775
F 0 "R10" V 4175 4775 50  0000 C CNN
F 1 "100" V 4275 4775 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 4775 50  0001 C CNN
F 3 "~" H 4275 4775 50  0001 C CNN
	1    4275 4775
	0    1    1    0   
$EndComp
Text GLabel 4425 4775 2    50   BiDi ~ 0
RD0
$Comp
L Device:R R11
U 1 1 60DEC098
P 4275 5075
F 0 "R11" V 4175 5075 50  0000 C CNN
F 1 "100" V 4275 5075 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 5075 50  0001 C CNN
F 3 "~" H 4275 5075 50  0001 C CNN
	1    4275 5075
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 60DEC2EC
P 4275 5375
F 0 "R12" V 4175 5375 50  0000 C CNN
F 1 "100" V 4275 5375 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 5375 50  0001 C CNN
F 3 "~" H 4275 5375 50  0001 C CNN
	1    4275 5375
	0    1    1    0   
$EndComp
$Comp
L Device:R R13
U 1 1 60DEC4FC
P 4275 5675
F 0 "R13" V 4175 5675 50  0000 C CNN
F 1 "100" V 4275 5675 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 5675 50  0001 C CNN
F 3 "~" H 4275 5675 50  0001 C CNN
	1    4275 5675
	0    1    1    0   
$EndComp
$Comp
L Device:R R14
U 1 1 60DEC726
P 4275 5975
F 0 "R14" V 4175 5975 50  0000 C CNN
F 1 "100" V 4275 5975 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 5975 50  0001 C CNN
F 3 "~" H 4275 5975 50  0001 C CNN
	1    4275 5975
	0    1    1    0   
$EndComp
$Comp
L Device:R R15
U 1 1 60DEC995
P 4275 6275
F 0 "R15" V 4175 6275 50  0000 C CNN
F 1 "100" V 4275 6275 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 6275 50  0001 C CNN
F 3 "~" H 4275 6275 50  0001 C CNN
	1    4275 6275
	0    1    1    0   
$EndComp
$Comp
L Device:R R16
U 1 1 60DECBF3
P 4275 6575
F 0 "R16" V 4175 6575 50  0000 C CNN
F 1 "100" V 4275 6575 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 6575 50  0001 C CNN
F 3 "~" H 4275 6575 50  0001 C CNN
	1    4275 6575
	0    1    1    0   
$EndComp
$Comp
L Device:R R17
U 1 1 60DECEDA
P 4275 6875
F 0 "R17" V 4175 6875 50  0000 C CNN
F 1 "100" V 4275 6875 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4205 6875 50  0001 C CNN
F 3 "~" H 4275 6875 50  0001 C CNN
	1    4275 6875
	0    1    1    0   
$EndComp
Text GLabel 4125 5075 0    50   BiDi ~ 0
D1
Text GLabel 4125 5375 0    50   BiDi ~ 0
D2
Text GLabel 4125 5675 0    50   BiDi ~ 0
D3
Text GLabel 4125 5975 0    50   BiDi ~ 0
D4
Text GLabel 4125 6275 0    50   BiDi ~ 0
D5
Text GLabel 4125 6575 0    50   BiDi ~ 0
D6
Text GLabel 4125 6875 0    50   BiDi ~ 0
D7
Text GLabel 4425 5075 2    50   BiDi ~ 0
RD1
Text GLabel 4425 5375 2    50   BiDi ~ 0
RD2
Text GLabel 4425 5675 2    50   BiDi ~ 0
RD3
Text GLabel 4425 5975 2    50   BiDi ~ 0
RD4
Text GLabel 4425 6275 2    50   BiDi ~ 0
RD5
Text GLabel 4425 6575 2    50   BiDi ~ 0
RD6
Text GLabel 4425 6875 2    50   BiDi ~ 0
RD7
Text GLabel 5625 2250 2    50   BiDi ~ 0
RD0
Text GLabel 5625 2350 2    50   BiDi ~ 0
RD1
Text GLabel 5625 2450 2    50   BiDi ~ 0
RD2
Text GLabel 5625 2650 2    50   BiDi ~ 0
RD3
Text GLabel 5625 2750 2    50   BiDi ~ 0
RD4
Text GLabel 5625 2850 2    50   BiDi ~ 0
RD5
Text GLabel 5625 2950 2    50   BiDi ~ 0
RD6
Text GLabel 5625 3050 2    50   BiDi ~ 0
RD7
Text GLabel 5625 1650 2    50   BiDi ~ 0
RnIOW
Text GLabel 5625 1750 2    50   BiDi ~ 0
RnIOR
Text GLabel 5000 4775 0    50   BiDi ~ 0
nIOW
Text GLabel 5000 5075 0    50   BiDi ~ 0
nIOR
$Comp
L Device:R R20
U 1 1 60DEE51A
P 5150 4775
F 0 "R20" V 5050 4775 50  0000 C CNN
F 1 "100" V 5150 4775 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5080 4775 50  0001 C CNN
F 3 "~" H 5150 4775 50  0001 C CNN
	1    5150 4775
	0    1    1    0   
$EndComp
$Comp
L Device:R R21
U 1 1 60DEE7FB
P 5150 5075
F 0 "R21" V 5050 5075 50  0000 C CNN
F 1 "100" V 5150 5075 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5080 5075 50  0001 C CNN
F 3 "~" H 5150 5075 50  0001 C CNN
	1    5150 5075
	0    1    1    0   
$EndComp
Text GLabel 5300 5075 2    50   BiDi ~ 0
RnIOR
Text GLabel 5300 4775 2    50   BiDi ~ 0
RnIOW
Text GLabel 2500 2025 2    50   BiDi ~ 0
GND
$EndSCHEMATC
