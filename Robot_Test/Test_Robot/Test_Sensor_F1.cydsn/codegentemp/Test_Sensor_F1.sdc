# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Henry Alberto\Desktop\Test_Robot\Test_Sensor_F1.cydsn\Test_Sensor_F1.cyprj
# Date: Thu, 05 Jan 2017 01:44:31 GMT
#set_units -time ns
create_clock -name {I2C_1_SCBCLK(FFB)} -period 625 -waveform {0 312.5} [list [get_pins {ClockBlock/ff_div_3}]]
create_clock -name {CyRouted1} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFCLK} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySYSCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {I2C_1_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 31 61} [list]


# Component constraints for C:\Users\Henry Alberto\Desktop\Test_Robot\Test_Sensor_F1.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Henry Alberto\Desktop\Test_Robot\Test_Sensor_F1.cydsn\Test_Sensor_F1.cyprj
# Date: Thu, 05 Jan 2017 01:44:26 GMT
