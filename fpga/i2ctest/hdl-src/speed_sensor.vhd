library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity speed_sensor is
  generic (
    pulses_per_revolution: integer := 4;
    clocks_per_second: integer := 50_000_000);
  port (
    clk_in: in std_logic;
    pulse: in std_logic;
    -- clock cycles per rotation
    speed: out unsigned(31 downto 0) := (others => '0'));
end speed_sensor;


architecture impl of speed_sensor is
  component edge_detector
    port (CLK: in std_logic;
          A: in std_logic;
          R: out std_logic;
          F: out std_logic);
  end component;

  signal edgecount: integer := 0;
  signal clockcount: integer := 0;
  signal edge: std_logic;
  signal state: std_logic := '0';
begin

  dect: edge_detector port map (
    CLK => clk_in,
    A => pulse,
    R => edge);
  
  process(clk_in)
  begin
    if rising_edge(clk_in) then
      clockcount <= clockcount + 1;
      if edge = '1' then
        edgecount <= edgecount + 1;
      end if;
      -- One tenth of a second has passed.
      if edgecount = pulses_per_revolution then
        speed <= conv_unsigned(clockcount, 32);
        edgecount <= 0;
        clockcount <= 0;
      elsif edgecount = clocks_per_second then
        speed <= conv_unsigned(clocks_per_second,32);
        edgecount <= 0;
        clockcount <= 0;
      end if;
    end if;
  end process;
end impl;
