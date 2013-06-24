library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity speed_sensor is
  generic (clock_hz_div_10: unsigned(31 downto 0) := conv_unsigned(5_000_000, 32));
  port (
    clk_in: in std_logic;
    pulse: in std_logic;
    -- in rotations per second
    speed: out unsigned(31 downto 0) := (others => '0'));
end speed_sensor;


architecture impl of speed_sensor is
  component edge_detector
    port (clock: in std_logic;
          input: in std_logic;
          output: out std_logic);
  end component;
  -- Every turn makes 4 edges.
  signal edgecount: unsigned(31 downto 0) := (others => '0');
  signal clockcount: unsigned(31 downto 0) := (others => '0');
  signal edge: std_logic;
begin

  dect: edge_detector port map (
    clock => clk_in,
    input => pulse,
    output => edge);
  

  process(clk_in)
  begin
    if rising_edge(clk_in) then
      clockcount <= clockcount + 1;
      if edge = '1' then
        edgecount <= edgecount + 1;
      end if;
      -- One tenth of a second has passed.
      if clockcount = clock_hz_div_10 then
        -- 4 edges per revolution, divide by 4
        speed <= "00" & edgecount(31 downto 2);
        edgecount <= conv_unsigned(0,32);
        clockcount <= conv_unsigned(0,32);
      end if;
    end if;
  end process;
end impl;
