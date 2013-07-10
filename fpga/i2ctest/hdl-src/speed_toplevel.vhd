library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity speed_toplevel is
  port (
    KEY: in std_logic_vector(3 downto 0);
    pulse_front: in std_logic;
    pulse_back: in std_logic;
    i2c_scl: inout std_logic;
    i2c_sda: inout std_logic;
    CLOCK_50: in std_logic);
end speed_toplevel;


architecture synth of speed_toplevel is
  component speed_sensor is
    generic (pulses_per_revolution: integer := 4;
             clocks_per_second: integer := 50_000_000);
    port (
      clk_in: in std_logic;
      pulse: in std_logic;
      -- in rotations per second
      speed: out unsigned(31 downto 0) := (others => '0'));
  end component;

  component motor_controller is
    generic (
      slave_address : std_logic_vector(6 downto 0));
    port (
      i2c_scl  : inout std_logic;
      i2c_sda  : inout std_logic;
      CLOCK_50 : in    std_logic;
      start    : in    std_logic;
      running  : out   std_logic;
      motor    : in    std_logic_vector(2 downto 0);
      speed    : in    unsigned(15 downto 0));
  end component motor_controller;

  component speed_control is
    generic (
      control_clock_divider : integer);
    port (
      CLOCK_50            : in  std_logic;
      speed_front         : in  unsigned(31 downto 0);
      speed_back          : in  unsigned(31 downto 0);
      desired_speed       : in  unsigned(31 downto 0);
      output_acceleration : out signed(31 downto 0) := (others => '0'));
  end component speed_control;

  signal start: std_logic := '0';
  signal running: std_logic := '0';
  signal motor: std_logic_vector(2 downto 0) := (others => '0');
  signal speed: unsigned(0 to 15) := (others => '0');

  signal speed_front: unsigned(31 downto 0) := (others => '0');
  signal speed_back: unsigned(31 downto 0) := (others => '0');

  signal desired_speed: unsigned(31 downto 0) := (others => '0');
  signal output_acceleration: signed(31 downto 0) := (others => '0');

  signal current_speed: unsigned(15 downto 0) := (others => '0');
  
begin
  mot_controller: motor_controller
    generic map (slave_address => "0000011")
    port map (
      CLOCK_50 => CLOCK_50,
      start => start,
      running => running,
      motor => motor,
      speed => current_speed,
      i2c_scl => i2c_scl,
      i2c_sda => i2c_sda);

  sensor_front: speed_sensor
    generic map (pulses_per_revolution => 4, clocks_per_second => 50_000_000)
    port map (clk_in => CLOCK_50, pulse => pulse_front, speed => speed_front);

  sensor_back: speed_sensor
    generic map (pulses_per_revolution => 4, clocks_per_second => 50_000_000)
    port map (clk_in => CLOCK_50, pulse => pulse_back, speed => speed_back);

  speed_cont: speed_control
    generic map (control_clock_divider => 4)
    port map (CLOCK_50 => CLOCK_50, speed_front => speed_back, speed_back => speed_front, desired_speed => desired_speed, output_acceleration => output_acceleration);

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      current_speed <= to_unsigned(to_integer(current_speed) - to_integer(output_acceleration), 16);
      if (current_speed < 0) then
        current_speed <= to_unsigned(0,16);
      elsif (current_speed > to_unsigned(7999,32)) then
        current_speed <= to_unsigned(8000, 16);
      end if;
    end if;
  end process;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if running = '0' then
        start <= '1';
        motor <= (others => '0');
      else
        start <= '0';
      end if;
    end if;
  end process;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if KEY(0) = '0' then
        desired_speed <= to_unsigned(0,32);
      elsif KEY(1) = '0' then
        desired_speed <= to_unsigned(23000000,32);
      end if;
    end if;
  end process;

end synth;
