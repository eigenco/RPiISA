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
P 1600 2475
F 0 "J1" H 1600 4242 50  0000 C CNN
F 1 "Bus_ISA_8bit" H 1600 4151 50  0000 C CNN
F 2 "adlib:BUS_PC" H 1600 2475 50  0001 C CNN
F 3 "https://en.wikipedia.org/wiki/Industry_Standard_Architecture" H 1600 2475 50  0001 C CNN
	1    1600 2475
	1    0    0    -1  
$EndComp
Text GLabel 2300 3075 2    50   Input ~ 0
A9
Text GLabel 2300 3175 2    50   Input ~ 0
A8
Text GLabel 2300 3275 2    50   Input ~ 0
A7
Text GLabel 2300 3375 2    50   Input ~ 0
A6
Text GLabel 2300 3475 2    50   Input ~ 0
A5
Text GLabel 2300 3575 2    50   Input ~ 0
A4
Text GLabel 2300 3675 2    50   Input ~ 0
A3
Text GLabel 2300 3775 2    50   Input ~ 0
A2
Text GLabel 2300 3875 2    50   Input ~ 0
A1
Text GLabel 2300 3975 2    50   Input ~ 0
A0
Text GLabel 900  1175 0    50   UnSpc ~ 0
VCC
Text GLabel 900  2175 0    50   Input ~ 0
IOW
Text GLabel 900  2275 0    50   Input ~ 0
IOR
$Comp
L Connector:Raspberry_Pi_2_3 J2
U 1 1 60DDB778
P 4325 2450
F 0 "J2" H 4325 4100 50  0000 C CNN
F 1 "Raspberry_Pi_2_3" H 4325 4025 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x20_P2.54mm_Vertical" H 4325 2450 50  0001 C CNN
F 3 "https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/rpi_SCH_3bplus_1p0_reduced.pdf" H 4325 2450 50  0001 C CNN
	1    4325 2450
	1    0    0    -1  
$EndComp
Text GLabel 3925 3750 3    50   UnSpc ~ 0
GND
Text GLabel 3525 2450 0    50   Output ~ 0
A0
Text GLabel 3525 2350 0    50   Output ~ 0
A1
Text GLabel 3525 1850 0    50   Output ~ 0
A3
Text GLabel 3525 2250 0    50   Output ~ 0
A4
Text GLabel 5125 3250 2    50   Output ~ 0
A5
Text GLabel 5125 3150 2    50   Output ~ 0
A6
Text GLabel 5125 2350 2    50   Output ~ 0
A7
Text GLabel 5125 2250 2    50   Output ~ 0
A8
Text GLabel 5125 1650 2    50   Output ~ 0
A9
Text GLabel 2300 1775 2    50   BiDi ~ 0
D0
Text GLabel 2300 1675 2    50   BiDi ~ 0
D1
Text GLabel 2300 1575 2    50   BiDi ~ 0
D2
Text GLabel 2300 1475 2    50   BiDi ~ 0
D3
Text GLabel 2300 1375 2    50   BiDi ~ 0
D4
Text GLabel 2300 1275 2    50   BiDi ~ 0
D5
Text GLabel 2300 1175 2    50   BiDi ~ 0
D6
Text GLabel 2300 1075 2    50   BiDi ~ 0
D7
Text GLabel 4025 3750 3    50   UnSpc ~ 0
GND
Text GLabel 4125 3750 3    50   UnSpc ~ 0
GND
Text GLabel 4225 3750 3    50   UnSpc ~ 0
GND
Text GLabel 4325 3750 3    50   UnSpc ~ 0
GND
Text GLabel 4425 3750 3    50   UnSpc ~ 0
GND
Text GLabel 4525 3750 3    50   UnSpc ~ 0
GND
Text GLabel 4625 3750 3    50   UnSpc ~ 0
GND
Text GLabel 900  975  0    50   UnSpc ~ 0
GND
Text GLabel 4125 1150 1    50   UnSpc ~ 0
VCC
Text GLabel 4225 1150 1    50   UnSpc ~ 0
VCC
Text GLabel 900  1875 0    50   UnSpc ~ 0
GND
Text GLabel 900  3775 0    50   UnSpc ~ 0
VCC
Text GLabel 900  3975 0    50   UnSpc ~ 0
GND
Text GLabel 7850 1825 0    50   BiDi ~ 0
D0
$Comp
L Device:R R10
U 1 1 60DEBC37
P 8000 1825
F 0 "R10" V 7900 1825 50  0000 C CNN
F 1 "100" V 8000 1825 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 1825 50  0001 C CNN
F 3 "~" H 8000 1825 50  0001 C CNN
	1    8000 1825
	0    1    1    0   
$EndComp
Text GLabel 8150 1825 2    50   BiDi ~ 0
RD0
$Comp
L Device:R R11
U 1 1 60DEC098
P 8000 2025
F 0 "R11" V 7900 2025 50  0000 C CNN
F 1 "100" V 8000 2025 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 2025 50  0001 C CNN
F 3 "~" H 8000 2025 50  0001 C CNN
	1    8000 2025
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 60DEC2EC
P 8000 2225
F 0 "R12" V 7900 2225 50  0000 C CNN
F 1 "100" V 8000 2225 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 2225 50  0001 C CNN
F 3 "~" H 8000 2225 50  0001 C CNN
	1    8000 2225
	0    1    1    0   
$EndComp
$Comp
L Device:R R13
U 1 1 60DEC4FC
P 8000 2425
F 0 "R13" V 7900 2425 50  0000 C CNN
F 1 "100" V 8000 2425 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 2425 50  0001 C CNN
F 3 "~" H 8000 2425 50  0001 C CNN
	1    8000 2425
	0    1    1    0   
$EndComp
$Comp
L Device:R R14
U 1 1 60DEC726
P 8000 2625
F 0 "R14" V 7900 2625 50  0000 C CNN
F 1 "100" V 8000 2625 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 2625 50  0001 C CNN
F 3 "~" H 8000 2625 50  0001 C CNN
	1    8000 2625
	0    1    1    0   
$EndComp
$Comp
L Device:R R15
U 1 1 60DEC995
P 8000 2825
F 0 "R15" V 7900 2825 50  0000 C CNN
F 1 "100" V 8000 2825 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 2825 50  0001 C CNN
F 3 "~" H 8000 2825 50  0001 C CNN
	1    8000 2825
	0    1    1    0   
$EndComp
$Comp
L Device:R R16
U 1 1 60DECBF3
P 8000 3025
F 0 "R16" V 7900 3025 50  0000 C CNN
F 1 "100" V 8000 3025 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 3025 50  0001 C CNN
F 3 "~" H 8000 3025 50  0001 C CNN
	1    8000 3025
	0    1    1    0   
$EndComp
$Comp
L Device:R R17
U 1 1 60DECEDA
P 8000 3225
F 0 "R17" V 7900 3225 50  0000 C CNN
F 1 "100" V 8000 3225 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7930 3225 50  0001 C CNN
F 3 "~" H 8000 3225 50  0001 C CNN
	1    8000 3225
	0    1    1    0   
$EndComp
Text GLabel 7850 2025 0    50   BiDi ~ 0
D1
Text GLabel 7850 2225 0    50   BiDi ~ 0
D2
Text GLabel 7850 2425 0    50   BiDi ~ 0
D3
Text GLabel 7850 2625 0    50   BiDi ~ 0
D4
Text GLabel 7850 2825 0    50   BiDi ~ 0
D5
Text GLabel 7850 3025 0    50   BiDi ~ 0
D6
Text GLabel 7850 3225 0    50   BiDi ~ 0
D7
Text GLabel 8150 2025 2    50   BiDi ~ 0
RD1
Text GLabel 8150 2225 2    50   BiDi ~ 0
RD2
Text GLabel 8150 2425 2    50   BiDi ~ 0
RD3
Text GLabel 8150 2625 2    50   BiDi ~ 0
RD4
Text GLabel 8150 2825 2    50   BiDi ~ 0
RD5
Text GLabel 8150 3025 2    50   BiDi ~ 0
RD6
Text GLabel 8150 3225 2    50   BiDi ~ 0
RD7
Text GLabel 3525 3150 0    50   BiDi ~ 0
RD0
Text GLabel 3525 2050 0    50   BiDi ~ 0
RD1
Text GLabel 3525 1950 0    50   BiDi ~ 0
RD2
Text GLabel 3525 1650 0    50   BiDi ~ 0
RD3
Text GLabel 3525 1550 0    50   BiDi ~ 0
RD4
Text GLabel 5125 2150 2    50   BiDi ~ 0
RD5
Text GLabel 5125 1950 2    50   BiDi ~ 0
RD6
Text GLabel 5125 1850 2    50   BiDi ~ 0
RD7
Text GLabel 3525 2950 0    50   Output ~ 0
IOW
Text GLabel 5125 2950 2    50   Output ~ 0
IOR
Text GLabel 900  1075 0    50   Input ~ 0
RESET
Text GLabel 3525 2650 0    50   Output ~ 0
RESET
Text GLabel 2300 1975 2    50   UnSpc ~ 0
GND
$Comp
L SamacSys_Parts:ACH-14.31818MHZ-EK Y1
U 1 1 6142166C
P 4700 4850
F 0 "Y1" H 5550 5115 50  0000 C CNN
F 1 "ACH-14.31818MHZ-EK" H 5550 5024 50  0000 C CNN
F 2 "SamacSys_Parts:ACH16000MHZEK" H 6250 4950 50  0001 L CNN
F 3 "" H 6250 4850 50  0001 L CNN
F 4 "Standard Clock Oscillators 14.31818MHz 5V" H 6250 4750 50  0001 L CNN "Description"
F 5 "6.39" H 6250 4650 50  0001 L CNN "Height"
F 6 "815-ACH-14.31818EK" H 6250 4550 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ABRACON/ACH-1431818MHZ-EK?qs=m%252BUhWDcpCfYHkLvwOoxiTw%3D%3D" H 6250 4450 50  0001 L CNN "Mouser Price/Stock"
F 8 "ABRACON" H 6250 4350 50  0001 L CNN "Manufacturer_Name"
F 9 "ACH-14.31818MHZ-EK" H 6250 4250 50  0001 L CNN "Manufacturer_Part_Number"
	1    4700 4850
	1    0    0    -1  
$EndComp
Text GLabel 4700 4850 0    50   UnSpc ~ 0
VCC
Text GLabel 6400 4950 2    50   UnSpc ~ 0
GND
Text GLabel 6400 4850 2    50   Output ~ 0
OSC
Text GLabel 900  3875 0    50   Input ~ 0
OSC
Text GLabel 900  1975 0    50   Input ~ 0
MEMW
Text GLabel 900  2075 0    50   Input ~ 0
MEMR
Text GLabel 2300 2975 2    50   Input ~ 0
A10
Text GLabel 2300 2875 2    50   Input ~ 0
A11
Text GLabel 2300 2775 2    50   Input ~ 0
A12
Text GLabel 2300 2675 2    50   Input ~ 0
A13
Text GLabel 2300 2575 2    50   Input ~ 0
A14
Text GLabel 2300 2475 2    50   Input ~ 0
A15
Text GLabel 2300 2375 2    50   Input ~ 0
A16
Text GLabel 2300 2275 2    50   Input ~ 0
A17
Text GLabel 2300 2175 2    50   Input ~ 0
A18
Text GLabel 2300 2075 2    50   Input ~ 0
A19
$Comp
L Connector_Generic:Conn_02x10_Counter_Clockwise J3
U 1 1 61421B75
P 6500 2550
F 0 "J3" H 6550 3167 50  0000 C CNN
F 1 "Conn_02x10_Counter_Clockwise" H 6550 3076 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm_Socket" H 6500 2550 50  0001 C CNN
F 3 "~" H 6500 2550 50  0001 C CNN
	1    6500 2550
	1    0    0    -1  
$EndComp
Text GLabel 6300 2150 0    50   UnSpc ~ 0
GND
Text GLabel 6300 2250 0    50   Input ~ 0
A0
Text GLabel 6300 2350 0    50   Input ~ 0
A1
Text GLabel 6300 2450 0    50   Input ~ 0
A2
Text GLabel 6300 2550 0    50   Input ~ 0
A3
Text GLabel 6300 2650 0    50   Input ~ 0
A4
Text GLabel 6300 2750 0    50   Input ~ 0
A5
Text GLabel 6300 2850 0    50   Input ~ 0
A6
Text GLabel 6300 2950 0    50   Input ~ 0
A7
Text GLabel 6800 2250 2    50   Output ~ 0
A12
Text GLabel 6800 2350 2    50   Output ~ 0
A13
Text GLabel 6800 2750 2    50   Output ~ 0
A17
Text GLabel 6800 2650 2    50   Output ~ 0
A16
Text GLabel 6800 2550 2    50   Output ~ 0
A15
Text GLabel 6800 2450 2    50   Output ~ 0
A14
Text GLabel 6300 3050 0    50   UnSpc ~ 0
GND
Text GLabel 6800 2150 2    50   UnSpc ~ 0
VCC
Text GLabel 3525 2750 0    50   Output ~ 0
LA
Text GLabel 6800 3050 2    50   Input ~ 0
LA
Text GLabel 5125 1550 2    50   Output ~ 0
A10
Text GLabel 5125 2550 2    50   Output ~ 0
A11
Text GLabel 6800 2850 2    50   Output ~ 0
A18
Text GLabel 6800 2950 2    50   Output ~ 0
A19
Text GLabel 5125 2750 2    50   Output ~ 0
MEMR
Text GLabel 5125 2850 2    50   Output ~ 0
MEMW
Text GLabel 3525 3050 0    50   Output ~ 0
A2
$EndSCHEMATC
