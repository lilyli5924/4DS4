Copyright by Phil Kinsman, Adam Kinsman, Henry Ko and Nicola Nicolici
Developed for the Embedded Systems course (COE4DS4)
Department of Electrical and Computer Engineering
McMaster University, Ontario, Canada

This file contains instructions for creating a NIOS system that can be simulated using Modelsim

Top Level Toolflow
==================

The toolflow for hardware/software co-design using Altera Nios has 4 main tools:

1. Quartus (for the hardware project)
2. Qsys (for system generation)
3. Nios SDK (for the software project)
4. Modelsim-Altera (for simulating the system)

Usage of these tools is documented below. Steps for creating a new system from scratch using these 4 tools follow directly, i.e. what you will have to do for experiment 1.

1. Quartus
==========

- When creating a new system, a Quartus project should be created in the same way as in previous labs. Use "experiment1" as the project name. Ensure that the correct device (Cyclone II - EP2C35F672C6) is selected.

2. Qsys Builder
===============

- Once the project has been created, select "Qsys" from the "Tools" menu, this will launch the system design tool. Load the "experiment1.qsys" file.

- Generating the system
	-- Click the "Generation" tab
	-- Change the "Create simulation model" to "Verilog"
	-- Uncheck "Create HDL design files for synthesis" and "Create block symbol file (.bsf)"
	-- Save the system
	-- Click "Generate", wait for the message "Generate Completed"
	-- Click "Close"

- Pin assignments
	-- Pin assignments are not necessary here, since you will only be simulating the design

- Synthesis
	-- Synthesis is not necessary here, since you will only be simulating the design

3. Nios SDK
===========

-Nios SDK project import
	-- Launch the Nios SDK and switch the workspace to the appropriate experiment folder
	-- Import the project from the software subfolder (same way how it was done in labs 2/3/4/...)

- Setting up NIOS SDK for simulation
	-- Right click on "experiment1_bsp", and select "Properties"
	-- Select "Nios II BSP Properties" (you may have to wait for the settings to become active)
	-- Check the box that says "ModelSim only, no hardware support"
	
- To start ModelSim for simulation
	-- Choose "Project"->"Build All" to start compilation in NIOS
	-- Right click on "experiment1" in the project window
	-- Select "Run As" -> "Nios II ModelSim"
	-- When the "Run Configurations" window opens, under "Qsys Testbench Simulation Package Descriptor Filename" select "experiment1.spd" from the project folder
	-- Click "Run"
	-- After a short delay you should see Modelsim being launched
	
4. ModelSim-Altera
===========
- Compile the test bench module
	-- At the prompt of ModelSim, enter:
	 	--- "vlog -sv ../../../../../../../test_bench.v"
- Configure the simulation environment
	-- At the prompt of ModelSim, enter:
		--- "set TOP_LEVEL_NAME test_bench"
		--- "set SYSTEM_INSTANCE_NAME /DUT"
	-- Loading the design files
		-- At the prompt of ModelSim, enter "ld"
	-- Adding signals into the waveforms
	-- Run the "wave.do" file in the experiment folder, at the prompt of ModelSim, enter:
		--- "do ../../../../../../../wave.do"
- To start the simulation
	-- At the prompt of ModelSim, enter "run -all"
	-- The simulation should start, and you can see the output of the simulation in the ModelSim environment
	-- All of the $write statements have been prefaced by "TBINFO" so that they can be distinguished from the printf statements in the C code