
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity speed_sensor2 is
  port (
    clk_in: in std_logic;
    pulse: in std_logic;
    -- clock cycles per rotation
    speed: out unsigned(7 downto 0) := (others => '0'));
end speed_sensor2;


architecture impl of speed_sensor2 is
  component edge_detector
    port (CLK: in std_logic;
          A: in std_logic;
          R: out std_logic;
          F: out std_logic);
  end component;

  signal edgecount: integer := 0;
  signal clockcount: integer := 0;
  signal edger: std_logic;
  signal edgef: std_logic;
  signal state: std_logic := '0';
  signal clock_divider: integer := 5000000;
begin

  dect: edge_detector port map (
    CLK => clk_in,
    A => pulse,
    R => edger,
    F => edgef);
  
  process(clk_in)
  begin
    if rising_edge(clk_in) then

      clockcount <= clockcount + 1;
      if edger = '1' or edgef = '1' then
        edgecount <= edgecount + 1;
      end if;
      if clockcount >= clock_divider then
        clockcount <= 0;
        edgecount <= 0;
        if edgecount > 100 then -- this would be more than 46km/h
          speed <= to_unsigned(100,8);
        else
          speed <= to_unsigned(edgecount,8);
        end if;
      end if;
    end if;
  end process;
end impl;
