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
Text GLabel 8000 1725 2    50   BiDi ~ 0
D0
Text GLabel 8000 1625 2    50   BiDi ~ 0
D1
Text GLabel 8000 1525 2    50   BiDi ~ 0
D2
Text GLabel 8000 1425 2    50   BiDi ~ 0
D3
Text GLabel 8000 1325 2    50   BiDi ~ 0
D4
Text GLabel 8000 1225 2    50   BiDi ~ 0
D5
Text GLabel 8000 1125 2    50   BiDi ~ 0
D6
Text GLabel 8000 1025 2    50   BiDi ~ 0
D7
Text GLabel 8000 3925 2    50   UnSpc ~ 0
A0
Text GLabel 8000 3825 2    50   UnSpc ~ 0
A1
Text GLabel 8000 3725 2    50   UnSpc ~ 0
A2
Text GLabel 8000 3625 2    50   UnSpc ~ 0
A3
Text GLabel 8000 3525 2    50   UnSpc ~ 0
A4
Text GLabel 8000 3425 2    50   UnSpc ~ 0
A5
Text GLabel 8000 3325 2    50   UnSpc ~ 0
A6
Text GLabel 8000 3225 2    50   UnSpc ~ 0
A7
Text GLabel 8000 3125 2    50   UnSpc ~ 0
A8
Text GLabel 8000 3025 2    50   UnSpc ~ 0
A9
Text GLabel 6600 3925 0    50   UnSpc ~ 0
GND
Text GLabel 6600 3725 0    50   UnSpc ~ 0
VCC
Text GLabel 6600 1125 0    50   UnSpc ~ 0
VCC
Text GLabel 6600 1825 0    50   UnSpc ~ 0
GND
Text GLabel 6600 925  0    50   UnSpc ~ 0
GND
Text GLabel 6600 2225 0    50   Input ~ 0
~IOR
Text GLabel 6600 2125 0    50   Input ~ 0
~IOW
$Comp
L Connector:Bus_ISA_8bit J4
U 1 1 60DD7EBA
P 7300 2425
F 0 "J4" H 7300 4192 50  0001 C CNN
F 1 "Bus_ISA_8bit" H 7300 4101 50  0000 C CNN
F 2 "adlib:BUS_PC" H 7300 2425 50  0001 C CNN
F 3 "https://en.wikipedia.org/wiki/Industry_Standard_Architecture" H 7300 2425 50  0001 C CNN
	1    7300 2425
	1    0    0    -1  
$EndComp
Text GLabel 6600 3825 0    50   UnSpc ~ 0
OSC
$Comp
L 74xx:74LS688 U1
U 1 1 613879CB
P 3300 2600
F 0 "U1" H 3844 2646 50  0000 L CNN
F 1 "74LS688" H 3844 2555 50  0000 L CNN
F 2 "Package_DIP:DIP-20_W7.62mm_Socket" H 3300 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS688" H 3300 2600 50  0001 C CNN
	1    3300 2600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x12_Counter_Clockwise J1
U 1 1 6138AFAC
P 5050 2200
F 0 "J1" H 5100 2917 50  0000 C CNN
F 1 "Conn_02x12_Counter_Clockwise" H 5100 2826 50  0000 C CNN
F 2 "Package_SO:SOP-24_7.5x15.4mm_P1.27mm" H 5050 2200 50  0001 C CNN
F 3 "~" H 5050 2200 50  0001 C CNN
	1    5050 2200
	1    0    0    -1  
$EndComp
Text GLabel 4850 1700 0    50   UnSpc ~ 0
VCC
Text GLabel 4850 2800 0    50   UnSpc ~ 0
GND
Text GLabel 4850 2600 0    50   BiDi ~ 0
D0
Text GLabel 4850 2700 0    50   BiDi ~ 0
D1
Text GLabel 5350 2800 2    50   BiDi ~ 0
D2
Text GLabel 5350 2700 2    50   BiDi ~ 0
D3
Text GLabel 5350 2600 2    50   BiDi ~ 0
D4
Text GLabel 5350 2500 2    50   BiDi ~ 0
D5
Text GLabel 5350 2400 2    50   BiDi ~ 0
D6
Text GLabel 5350 2300 2    50   BiDi ~ 0
D7
Text GLabel 4850 2200 0    50   Input ~ 0
~IOW
Text GLabel 4850 2300 0    50   Input ~ 0
~IOR
Text GLabel 4850 2000 0    50   UnSpc ~ 0
A0
Text GLabel 4850 2100 0    50   UnSpc ~ 0
A1
Text GLabel 5350 1700 2    50   UnSpc ~ 0
OSC
Text GLabel 3800 1700 2    50   UnSpc ~ 0
~CS
Text GLabel 4850 2400 0    50   UnSpc ~ 0
~CS
Text GLabel 3300 1400 1    50   UnSpc ~ 0
VCC
Text GLabel 3300 3800 3    50   UnSpc ~ 0
GND
Text GLabel 2800 1700 0    50   UnSpc ~ 0
VCC
Text GLabel 2800 1800 0    50   UnSpc ~ 0
VCC
Text GLabel 2800 1900 0    50   UnSpc ~ 0
VCC
Text GLabel 2800 2000 0    50   UnSpc ~ 0
GND
Text GLabel 2800 2100 0    50   UnSpc ~ 0
GND
Text GLabel 2800 2200 0    50   UnSpc ~ 0
GND
Text GLabel 2800 2300 0    50   UnSpc ~ 0
VCC
Text GLabel 2800 2400 0    50   UnSpc ~ 0
GND
Text GLabel 2800 3300 0    50   UnSpc ~ 0
A2
Text GLabel 2800 3200 0    50   UnSpc ~ 0
A3
Text GLabel 2800 3100 0    50   UnSpc ~ 0
A4
Text GLabel 2800 3000 0    50   UnSpc ~ 0
A5
Text GLabel 2800 2900 0    50   UnSpc ~ 0
A6
Text GLabel 2800 2800 0    50   UnSpc ~ 0
A7
Text GLabel 2800 2700 0    50   UnSpc ~ 0
A8
Text GLabel 2800 2600 0    50   UnSpc ~ 0
A9
Text GLabel 4850 1900 0    50   UnSpc ~ 0
VCC
$Comp
L Connector_Generic:Conn_02x08_Counter_Clockwise J2
U 1 1 61390151
P 5050 3650
F 0 "J2" H 5100 4167 50  0000 C CNN
F 1 "Conn_02x08_Counter_Clockwise" H 5100 4076 50  0000 C CNN
F 2 "Package_SO:SOP-16_4.55x10.3mm_P1.27mm" H 5050 3650 50  0001 C CNN
F 3 "~" H 5050 3650 50  0001 C CNN
	1    5050 3650
	1    0    0    -1  
$EndComp
Text GLabel 4850 3350 0    50   UnSpc ~ 0
VCC
Text GLabel 4850 3450 0    50   UnSpc ~ 0
GND
Text GLabel 4850 3550 0    50   UnSpc ~ 0
VCC
Text GLabel 5350 1800 2    50   UnSpc ~ 0
DACCLK
Text GLabel 4850 3750 0    50   UnSpc ~ 0
DACCLK
Text GLabel 5350 2000 2    50   UnSpc ~ 0
DOAB
Text GLabel 4850 3650 0    50   UnSpc ~ 0
DOAB
Text GLabel 5350 2100 2    50   UnSpc ~ 0
SMPAC
Text GLabel 4850 3950 0    50   UnSpc ~ 0
SMPAC
Text GLabel 5350 2200 2    50   UnSpc ~ 0
SMPBD
Text GLabel 4850 4050 0    50   UnSpc ~ 0
SMPBD
Text GLabel 5350 3950 2    50   UnSpc ~ 0
CH1
Text GLabel 5350 3850 2    50   UnSpc ~ 0
CH2
Text GLabel 5350 3350 2    50   UnSpc ~ 0
CV
Text GLabel 5350 3550 2    50   UnSpc ~ 0
MP
Text GLabel 5350 3650 2    50   UnSpc ~ 0
AOUT
Text GLabel 5350 3750 2    50   UnSpc ~ 0
SWIN
Text GLabel 5350 4050 2    50   UnSpc ~ 0
VCC
Text GLabel 4850 3850 0    50   UnSpc ~ 0
VCC
$Comp
L Connector_Generic:Conn_02x03_Counter_Clockwise J3
U 1 1 613887D7
P 5050 5675
F 0 "J3" H 5100 5992 50  0000 C CNN
F 1 "Conn_02x03_Counter_Clockwise" H 5100 5901 50  0000 C CNN
F 2 "SamacSys_Parts:STX31003N" H 5050 5675 50  0001 C CNN
F 3 "~" H 5050 5675 50  0001 C CNN
	1    5050 5675
	1    0    0    -1  
$EndComp
Text GLabel 4850 5575 0    50   UnSpc ~ 0
GND
$Comp
L Connector_Generic:Conn_02x07_Counter_Clockwise J5
U 1 1 6138BBC7
P 5050 4775
F 0 "J5" H 5100 5292 50  0000 C CNN
F 1 "Conn_02x07_Counter_Clockwise" H 5100 5201 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 5050 4775 50  0001 C CNN
F 3 "~" H 5050 4775 50  0001 C CNN
	1    5050 4775
	1    0    0    -1  
$EndComp
Text GLabel 6600 1725 0    50   UnSpc ~ 0
VP
Text GLabel 6600 1525 0    50   UnSpc ~ 0
VM
Text GLabel 4850 4775 0    50   UnSpc ~ 0
VP
Text GLabel 5350 4775 2    50   UnSpc ~ 0
VM
Text GLabel 4850 4675 0    50   UnSpc ~ 0
CV
Text GLabel 4850 4875 0    50   UnSpc ~ 0
AOUT
Text GLabel 5350 4875 2    50   UnSpc ~ 0
CH1
Text GLabel 5350 4675 2    50   UnSpc ~ 0
CH2
Text GLabel 5350 5075 2    50   UnSpc ~ 0
OUT3
Text GLabel 5350 4475 2    50   UnSpc ~ 0
OUT4
Text GLabel 4850 5675 0    50   UnSpc ~ 0
OUT3
Text GLabel 5350 5675 2    50   UnSpc ~ 0
OUT4
$Comp
L Device:R R1
U 1 1 6138F434
P 6900 4800
F 0 "R1" H 6970 4846 50  0001 L CNN
F 1 "33" H 6970 4800 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6830 4800 50  0001 C CNN
F 3 "~" H 6900 4800 50  0001 C CNN
	1    6900 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 6138F966
P 6050 4800
F 0 "C1" H 6165 4846 50  0001 L CNN
F 1 "10uF" H 6165 4800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6088 4650 50  0001 C CNN
F 3 "~" H 6050 4800 50  0001 C CNN
	1    6050 4800
	1    0    0    -1  
$EndComp
Text GLabel 6050 4650 1    50   UnSpc ~ 0
CV
Text GLabel 6050 4950 3    50   UnSpc ~ 0
GND
Text GLabel 4850 4575 0    50   UnSpc ~ 0
MP
Text GLabel 4850 4475 0    50   UnSpc ~ 0
MP
$Comp
L Device:C C2
U 1 1 61390916
P 6475 4800
F 0 "C2" H 6590 4846 50  0001 L CNN
F 1 "52pF" H 6590 4800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6513 4650 50  0001 C CNN
F 3 "~" H 6475 4800 50  0001 C CNN
	1    6475 4800
	1    0    0    -1  
$EndComp
Text GLabel 6475 4950 3    50   UnSpc ~ 0
GND
Text GLabel 6475 4650 1    50   UnSpc ~ 0
AOUT
Text GLabel 6900 4650 1    50   UnSpc ~ 0
SWIN
Text GLabel 4850 5075 0    50   UnSpc ~ 0
OUT2
Text GLabel 6900 4950 3    50   UnSpc ~ 0
OUT2
Text GLabel 4850 4975 0    50   UnSpc ~ 0
OUT2
$Comp
L Device:C C3
U 1 1 6139164A
P 7300 4800
F 0 "C3" H 7415 4846 50  0001 L CNN
F 1 "2.7nF" H 7415 4800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7338 4650 50  0001 C CNN
F 3 "~" H 7300 4800 50  0001 C CNN
	1    7300 4800
	1    0    0    -1  
$EndComp
Text GLabel 7300 4950 3    50   UnSpc ~ 0
GND
Text GLabel 7300 4650 1    50   UnSpc ~ 0
CH1
$Comp
L Device:C C4
U 1 1 61391B13
P 7725 4800
F 0 "C4" H 7840 4846 50  0001 L CNN
F 1 "2.7nF" H 7840 4800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7763 4650 50  0001 C CNN
F 3 "~" H 7725 4800 50  0001 C CNN
	1    7725 4800
	1    0    0    -1  
$EndComp
Text GLabel 7725 4950 3    50   UnSpc ~ 0
GND
Text GLabel 7725 4650 1    50   UnSpc ~ 0
CH2
Text GLabel 5350 4975 2    50   UnSpc ~ 0
OUT3
Text GLabel 5350 4575 2    50   UnSpc ~ 0
OUT4
$Comp
L Device:C C5
U 1 1 6139251A
P 8175 4800
F 0 "C5" H 8290 4846 50  0001 L CNN
F 1 "10uF" H 8290 4800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8213 4650 50  0001 C CNN
F 3 "~" H 8175 4800 50  0001 C CNN
	1    8175 4800
	1    0    0    -1  
$EndComp
Text GLabel 8175 4950 3    50   UnSpc ~ 0
GND
Text GLabel 8175 4650 1    50   UnSpc ~ 0
VP
$Comp
L Device:C C6
U 1 1 61392D99
P 8600 4800
F 0 "C6" H 8715 4846 50  0001 L CNN
F 1 "10uF" H 8715 4800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8638 4650 50  0001 C CNN
F 3 "~" H 8600 4800 50  0001 C CNN
	1    8600 4800
	1    0    0    -1  
$EndComp
Text GLabel 8600 4950 3    50   UnSpc ~ 0
VM
Text GLabel 8600 4650 1    50   UnSpc ~ 0
GND
Text GLabel 2800 3500 0    50   UnSpc ~ 0
GND
$EndSCHEMATC
