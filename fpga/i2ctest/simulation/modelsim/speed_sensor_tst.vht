LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_logic_arith.all;

ENTITY speed_sensor_tst IS
END speed_sensor_tst;

ARCHITECTURE impl OF speed_sensor_tst IS
  COMPONENT speed_sensor is
    port (
      clk_in: in std_logic;
      pulse: in std_logic;
      -- in rotations per second
      speed: out unsigned);
  END COMPONENT;

  SIGNAL CLOCK_50 : STD_LOGIC := '0';
  SIGNAL pulse: STD_LOGIC := '0';
  SIGNAL speed: unsigned(31 downto 0) := (others => '0');
  
BEGIN
  i1 : speed_sensor
    PORT MAP (
      clk_in => CLOCK_50,
      pulse =>  pulse,
      speed => speed
      );

  PROCESS                                         
  BEGIN
    -- One tenth second simulation.
    for i in 0 to 15_000_000 loop
      CLOCK_50 <= '1';
      wait for 10 ns;
      CLOCK_50 <= not CLOCK_50;
      wait for 10 ns;
    end loop;
    WAIT;
  END PROCESS;

  PROCESS                                              
  BEGIN
    -- Simulate  a speed of one rotation per tenth second.
    for i in 0 to 7 loop
      pulse <= '1';
      wait for 6250 us;
      pulse <= '0';
      wait for 6250 us;
    end loop;
    for i in 0 to 15 loop
      pulse <= '1';
      wait for 3125 us;
      pulse <= '0';
      wait for 3125 us;
    end loop;
    WAIT;
  END PROCESS;

END impl;
