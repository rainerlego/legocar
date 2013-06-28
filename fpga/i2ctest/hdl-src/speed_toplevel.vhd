library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity speed_toplevel is
  port (
    LEDR: out std_logic_vector(18 downto 0) := (others => '0');
    pulse_1: in std_logic;
    pulse_2: in std_logic;
    CLOCK_50: in std_logic);
end speed_toplevel;


architecture synth of speed_toplevel is
  component speed_sensor is
    generic (clock_hz_div_10: unsigned(31 downto 0) := conv_unsigned(5_000_000, 32));
    port (
      clk_in: in std_logic;
      pulse: in std_logic;
      -- in rotations per second
      speed: out unsigned(31 downto 0) := (others => '0'));
  end component;

  signal speed_1: unsigned(31 downto 0) := (others => '0');
  signal speed_2: unsigned(31 downto 0) := (others => '0');
  signal diff: unsigned(31 downto 0) := (others => '0');
begin

  sensor1: speed_sensor port map(
    clk_in => CLOCK_50,
    pulse => pulse_1,
    speed => speed_1);

  sensor2: speed_sensor port map(
    clk_in => CLOCK_50,
    pulse => pulse_2,
    speed => speed_2);


  diff <= 10 + speed_1 - speed_2;
  LEDR(18 downto 0) <=  conv_std_logic_vector(diff(18 downto 0), 19);
end synth;

