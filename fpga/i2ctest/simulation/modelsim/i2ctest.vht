-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, Altera MegaCore Function License 
-- Agreement, or other applicable license agreement, including, 
-- without limitation, that your use is for the sole purpose of 
-- programming logic devices manufactured by Altera and sold by 
-- Altera or its authorized distributors.  Please refer to the 
-- applicable agreement for further details.

-- ***************************************************************************
-- This file contains a Vhdl test bench template that is freely editable to   
-- suit user's needs .Comments are provided in each section to help the user  
-- fill out necessary details.                                                
-- ***************************************************************************
-- Generated on "05/23/2013 15:41:18"
                                                            
-- Vhdl Test Bench template for design  :  i2ctest
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY i2ctest_vhd_tst IS
END i2ctest_vhd_tst;

ARCHITECTURE i2ctest_arch OF i2ctest_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL CLOCK_50 : STD_LOGIC;
SIGNAL i2c_scl : STD_LOGIC;
SIGNAL i2c_sda : STD_LOGIC;
SIGNAL KEY : STD_LOGIC_VECTOR(3 DOWNTO 0);
COMPONENT i2ctest
	PORT (
	CLOCK_50 : IN STD_LOGIC;
	i2c_scl : INOUT STD_LOGIC;
	i2c_sda : INOUT STD_LOGIC;
	KEY : IN STD_LOGIC_VECTOR(3 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : i2ctest
	PORT MAP (
-- list connections between master ports and signals
	CLOCK_50 => CLOCK_50,
	i2c_scl => i2c_scl,
	i2c_sda => i2c_sda,
	KEY => KEY
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                 
	KEY <= (others => '0');
	CLOCK_50 <= '1';
	wait for 5ns;
	CLOCK_50 <= '0';
	wait for 5ns;
	CLOCK_50 <= '1';
	wait for 5ns;
	CLOCK_50 <= '0';
	wait for 5ns;
	CLOCK_50 <= '1';
	KEY <= (others => '1');
	for i in 0 to 10000 loop
		wait for 5 ns;
		CLOCK_50 <= not CLOCK_50;
	end loop;
WAIT;                                       
END PROCESS init;                                           
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                     
BEGIN                                                         
WAIT;                                                        
END PROCESS always;                                          
END i2ctest_arch;
