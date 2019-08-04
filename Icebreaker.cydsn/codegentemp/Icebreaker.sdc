# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\dev\git\Icebreaker\Icebreaker.cydsn\Icebreaker.cyprj
# Date: Sun, 02 Dec 2018 09:46:36 GMT
#set_units -time ns
create_clock -name {Clk_Counter(FFB)} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/ff_div_11}] [get_pins {ClockBlock/ff_div_12}]]
create_clock -name {UART_SCBCLK(FFB)} -period 729.16666666666663 -waveform {0 364.583333333333} [list [get_pins {ClockBlock/ff_div_2}]]
create_clock -name {ADC_intClock(FFB)} -period 500 -waveform {0 250} [list [get_pins {ClockBlock/ff_div_10}]]
create_clock -name {CodecI2CM_SCBCLK(FFB)} -period 125 -waveform {0 62.5} [list [get_pins {ClockBlock/ff_div_3}]]
create_clock -name {CyRouted1} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyWCO} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/wco}]]
create_clock -name {CyLFCLK} -period 30517.578125 -waveform {0 15258.7890625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyPLL1} -period 44.288548752834473 -waveform {0 22.1442743764172} [list [get_pins {ClockBlock/dbl}]]
create_clock -name {CyRouted2} -period 44.288548752834473 -waveform {0 22.1442743764172} [list [get_pins {ClockBlock/dsi_in_1}]]
create_clock -name {CyPLL0} -period 40.690104166666664 -waveform {0 20.3450520833333} [list [get_pins {ClockBlock/pll}]]
create_clock -name {CyRouted3} -period 40.690104166666664 -waveform {0 20.3450520833333} [list [get_pins {ClockBlock/dsi_in_2}]]
create_clock -name {CyECO} -period 58.128720238095241 -waveform {0 29.0643601190476} [list [get_pins {ClockBlock/eco}]]
create_clock -name {CyIMO} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySYSCLK} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {Clk_Counter} -source [get_pins {ClockBlock/hfclk}] -edges {1 2 3} [list]
create_generated_clock -name {UART_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 35 71} [list]
create_generated_clock -name {ADC_intClock} -source [get_pins {ClockBlock/hfclk}] -edges {1 25 49} [list]
create_generated_clock -name {CodecI2CM_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 7 13} [list]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\dev\git\Icebreaker\Icebreaker.cydsn\TopDesign\TopDesign.cysch
# Project: C:\dev\git\Icebreaker\Icebreaker.cydsn\Icebreaker.cyprj
# Date: Sun, 02 Dec 2018 09:46:26 GMT
