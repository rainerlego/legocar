LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_logic_arith.all;

ENTITY speed_sensor_tst IS
END speed_sensor_tst;

ARCHITECTURE impl OF speed_sensor_tst IS
  component speed_sensor is
    generic (
      pulses_per_revolution : integer;
      clocks_per_second : integer);
    port (
      clk_in : in  std_logic;
      pulse  : in  std_logic;
      speed  : out unsigned(31 downto 0) := (others => '0'));
  end component speed_sensor;

  SIGNAL CLOCK_50 : STD_LOGIC := '0';
  SIGNAL pulse: STD_LOGIC := '0';
  SIGNAL speed: unsigned(31 downto 0) := (others => '0');
  
BEGIN
  i1 : speed_sensor
    generic map (pulses_per_revolution => 4, clocks_per_second => 50_000_000)
    PORT MAP (
      clk_in => CLOCK_50,
      pulse =>  pulse,
      speed => speed
      );

  PROCESS                                         
  BEGIN
    -- One tenth second simulation.
    for i in 0 to 25_000_000 loop
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
    for i in 0 to 4 loop
      pulse <= '1';
      wait for 100 ns;
      pulse <= '0';
      wait for 100 ns;
    end loop;
    WAIT;
  END PROCESS;

END impl;
