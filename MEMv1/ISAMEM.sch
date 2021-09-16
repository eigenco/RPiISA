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
P 2850 3625
F 0 "J1" H 2850 5392 50  0000 C CNN
F 1 "Bus_ISA_8bit" H 2850 5301 50  0000 C CNN
F 2 "adlib:BUS_PC" H 2850 3625 50  0001 C CNN
F 3 "https://en.wikipedia.org/wiki/Industry_Standard_Architecture" H 2850 3625 50  0001 C CNN
	1    2850 3625
	1    0    0    -1  
$EndComp
Text GLabel 3550 5125 2    50   Output ~ 0
A0
Text GLabel 3550 5025 2    50   Output ~ 0
A1
Text GLabel 3550 4925 2    50   Output ~ 0
A2
Text GLabel 3550 4825 2    50   Output ~ 0
A3
Text GLabel 3550 4725 2    50   Output ~ 0
A4
Text GLabel 3550 4625 2    50   Output ~ 0
A5
Text GLabel 3550 4525 2    50   Output ~ 0
A6
Text GLabel 3550 4425 2    50   Output ~ 0
A7
Text GLabel 3550 4325 2    50   Output ~ 0
A8
Text GLabel 3550 4225 2    50   Output ~ 0
A9
Text GLabel 3550 4125 2    50   Output ~ 0
A10
Text GLabel 3550 4025 2    50   Output ~ 0
A11
Text GLabel 3550 3925 2    50   Output ~ 0
A12
Text GLabel 3550 3825 2    50   Output ~ 0
A13
Text GLabel 3550 3725 2    50   Output ~ 0
A14
Text GLabel 3550 3625 2    50   Output ~ 0
A15
Text GLabel 3550 3525 2    50   Output ~ 0
A16
Text GLabel 3550 3425 2    50   Output ~ 0
A17
Text GLabel 3550 3325 2    50   Output ~ 0
A18
Text GLabel 3550 3225 2    50   Output ~ 0
A19
Text GLabel 3550 2925 2    50   BiDi ~ 0
D0
Text GLabel 3550 2825 2    50   BiDi ~ 0
D1
Text GLabel 3550 2725 2    50   BiDi ~ 0
D2
Text GLabel 3550 2625 2    50   BiDi ~ 0
D3
Text GLabel 3550 2525 2    50   BiDi ~ 0
D4
Text GLabel 3550 2425 2    50   BiDi ~ 0
D5
Text GLabel 3550 2325 2    50   BiDi ~ 0
D6
Text GLabel 3550 2225 2    50   BiDi ~ 0
D7
Text GLabel 2150 3125 0    50   Output ~ 0
~MEMW
Text GLabel 2150 3225 0    50   Output ~ 0
~MEMR
Text GLabel 2150 2325 0    50   UnSpc ~ 0
V5
Text GLabel 2150 5125 0    50   UnSpc ~ 0
GND
$Comp
L SamacSys_Parts:AS6C4008-55PCN IC3
U 1 1 60412523
P 6400 3000
F 0 "IC3" H 6900 3265 50  0000 C CNN
F 1 "AS6C4008-55PCN" H 6900 3174 50  0000 C CNN
F 2 "DIP1524W58P254L4191H406Q32N" H 7250 3100 50  0001 L CNN
F 3 "http://componentsearchengine.com/Datasheets/1/AS6C4008-55PCN.pdf" H 7250 3000 50  0001 L CNN
F 4 "CMOS SRAM" H 7250 2900 50  0001 L CNN "Description"
F 5 "4.064" H 7250 2800 50  0001 L CNN "Height"
F 6 "913-AS6C4008-55PCN" H 7250 2700 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Alliance-Memory/AS6C4008-55PCN/?qs=E5c5%252Bmu3i3%252BMOyro1Tlhzg%3D%3D" H 7250 2600 50  0001 L CNN "Mouser Price/Stock"
F 8 "Alliance Memory" H 7250 2500 50  0001 L CNN "Manufacturer_Name"
F 9 "AS6C4008-55PCN" H 7250 2400 50  0001 L CNN "Manufacturer_Part_Number"
	1    6400 3000
	1    0    0    -1  
$EndComp
Text GLabel 7400 4000 2    50   Input ~ 0
A19
Text GLabel 7400 3000 2    50   UnSpc ~ 0
V5
Text GLabel 6400 4200 0    50   BiDi ~ 0
D0
Text GLabel 6400 4300 0    50   BiDi ~ 0
D1
Text GLabel 6400 4400 0    50   BiDi ~ 0
D2
Text GLabel 6400 4500 0    50   UnSpc ~ 0
GND
Text GLabel 6400 4100 0    50   Input ~ 0
A0
Text GLabel 6400 4000 0    50   Input ~ 0
A1
Text GLabel 6400 3900 0    50   Input ~ 0
A2
Text GLabel 6400 3800 0    50   Input ~ 0
A3
Text GLabel 6400 3700 0    50   Input ~ 0
A4
Text GLabel 6400 3600 0    50   Input ~ 0
A5
Text GLabel 6400 3500 0    50   Input ~ 0
A6
Text GLabel 6400 3400 0    50   Input ~ 0
A7
Text GLabel 7400 3500 2    50   Input ~ 0
A8
Text GLabel 7400 3600 2    50   Input ~ 0
A9
Text GLabel 7400 3900 2    50   Input ~ 0
A10
Text GLabel 7400 3700 2    50   Input ~ 0
A11
Text GLabel 7400 3800 2    50   Input ~ 0
~MEMR
Text GLabel 7400 3300 2    50   Input ~ 0
~MEMW
Text GLabel 7400 3400 2    50   Input ~ 0
A13
Text GLabel 7400 4500 2    50   BiDi ~ 0
D3
Text GLabel 7400 4400 2    50   BiDi ~ 0
D4
Text GLabel 7400 4300 2    50   BiDi ~ 0
D5
Text GLabel 7400 4200 2    50   BiDi ~ 0
D6
Text GLabel 7400 4100 2    50   BiDi ~ 0
D7
Text GLabel 7400 3100 2    50   Input ~ 0
A15
Text GLabel 7400 3200 2    50   Input ~ 0
A17
Text GLabel 6400 3300 0    50   Input ~ 0
A12
Text GLabel 6400 3200 0    50   Input ~ 0
A14
Text GLabel 6400 3100 0    50   Input ~ 0
A16
Text GLabel 6400 3000 0    50   Input ~ 0
A18
Text GLabel 2150 4925 0    50   UnSpc ~ 0
V5
Text GLabel 2150 2125 0    50   UnSpc ~ 0
GND
Text GLabel 2150 3025 0    50   UnSpc ~ 0
GND
$Comp
L Device:C C1
U 1 1 60DD8211
P 5150 3750
F 0 "C1" H 5265 3796 50  0000 L CNN
F 1 "10uF" H 5265 3705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5188 3600 50  0001 C CNN
F 3 "~" H 5150 3750 50  0001 C CNN
	1    5150 3750
	1    0    0    -1  
$EndComp
Text GLabel 5150 3900 3    50   UnSpc ~ 0
GND
Text GLabel 5150 3600 1    50   UnSpc ~ 0
V5
$EndSCHEMATC
