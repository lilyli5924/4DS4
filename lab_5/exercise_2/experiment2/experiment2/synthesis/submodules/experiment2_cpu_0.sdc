# Legal Notice: (C)2020 Altera Corporation. All rights reserved.  Your
# use of Altera Corporation's design tools, logic functions and other
# software and tools, and its AMPP partner logic functions, and any
# output files any of the foregoing (including device programming or
# simulation files), and any associated documentation or information are
# expressly subject to the terms and conditions of the Altera Program
# License Subscription Agreement or other applicable license agreement,
# including, without limitation, that your use is for the sole purpose
# of programming logic devices manufactured by Altera and sold by Altera
# or its authorized distributors.  Please refer to the applicable
# agreement for further details.

#**************************************************************
# Timequest JTAG clock definition
#   Uncommenting the following lines will define the JTAG
#   clock in TimeQuest Timing Analyzer
#**************************************************************

#create_clock -period 10MHz {altera_internal_jtag|tckutap}
#set_clock_groups -asynchronous -group {altera_internal_jtag|tckutap}

#**************************************************************
# Set TCL Path Variables 
#**************************************************************

set 	experiment2_cpu_0 	experiment2_cpu_0:*
set 	experiment2_cpu_0_oci 	experiment2_cpu_0_nios2_oci:the_experiment2_cpu_0_nios2_oci
set 	experiment2_cpu_0_oci_break 	experiment2_cpu_0_nios2_oci_break:the_experiment2_cpu_0_nios2_oci_break
set 	experiment2_cpu_0_ocimem 	experiment2_cpu_0_nios2_ocimem:the_experiment2_cpu_0_nios2_ocimem
set 	experiment2_cpu_0_oci_debug 	experiment2_cpu_0_nios2_oci_debug:the_experiment2_cpu_0_nios2_oci_debug
set 	experiment2_cpu_0_wrapper 	experiment2_cpu_0_jtag_debug_module_wrapper:the_experiment2_cpu_0_jtag_debug_module_wrapper
set 	experiment2_cpu_0_jtag_tck 	experiment2_cpu_0_jtag_debug_module_tck:the_experiment2_cpu_0_jtag_debug_module_tck
set 	experiment2_cpu_0_jtag_sysclk 	experiment2_cpu_0_jtag_debug_module_sysclk:the_experiment2_cpu_0_jtag_debug_module_sysclk
set 	experiment2_cpu_0_oci_path 	 [format "%s|%s" $experiment2_cpu_0 $experiment2_cpu_0_oci]
set 	experiment2_cpu_0_oci_break_path 	 [format "%s|%s" $experiment2_cpu_0_oci_path $experiment2_cpu_0_oci_break]
set 	experiment2_cpu_0_ocimem_path 	 [format "%s|%s" $experiment2_cpu_0_oci_path $experiment2_cpu_0_ocimem]
set 	experiment2_cpu_0_oci_debug_path 	 [format "%s|%s" $experiment2_cpu_0_oci_path $experiment2_cpu_0_oci_debug]
set 	experiment2_cpu_0_jtag_tck_path 	 [format "%s|%s|%s" $experiment2_cpu_0_oci_path $experiment2_cpu_0_wrapper $experiment2_cpu_0_jtag_tck]
set 	experiment2_cpu_0_jtag_sysclk_path 	 [format "%s|%s|%s" $experiment2_cpu_0_oci_path $experiment2_cpu_0_wrapper $experiment2_cpu_0_jtag_sysclk]
set 	experiment2_cpu_0_jtag_sr 	 [format "%s|*sr" $experiment2_cpu_0_jtag_tck_path]

#**************************************************************
# Set False Paths
#**************************************************************

set_false_path -from [get_keepers *$experiment2_cpu_0_oci_break_path|break_readreg*] -to [get_keepers *$experiment2_cpu_0_jtag_sr*]
set_false_path -from [get_keepers *$experiment2_cpu_0_oci_debug_path|*resetlatch]     -to [get_keepers *$experiment2_cpu_0_jtag_sr[33]]
set_false_path -from [get_keepers *$experiment2_cpu_0_oci_debug_path|monitor_ready]  -to [get_keepers *$experiment2_cpu_0_jtag_sr[0]]
set_false_path -from [get_keepers *$experiment2_cpu_0_oci_debug_path|monitor_error]  -to [get_keepers *$experiment2_cpu_0_jtag_sr[34]]
set_false_path -from [get_keepers *$experiment2_cpu_0_ocimem_path|*MonDReg*] -to [get_keepers *$experiment2_cpu_0_jtag_sr*]
set_false_path -from *$experiment2_cpu_0_jtag_sr*    -to *$experiment2_cpu_0_jtag_sysclk_path|*jdo*
set_false_path -from sld_hub:*|irf_reg* -to *$experiment2_cpu_0_jtag_sysclk_path|ir*
set_false_path -from sld_hub:*|sld_shadow_jsm:shadow_jsm|state[1] -to *$experiment2_cpu_0_oci_debug_path|monitor_go
