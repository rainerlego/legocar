library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity speed_control is
  generic (control_clock_divider: integer := 4);
  port (CLOCK_50: in std_logic;
        speed_front: in unsigned(31 downto 0);
        speed_back: in unsigned(31 downto 0);
        desired_speed: in unsigned(31 downto 0);
        enable_antischlupf: in std_logic;
        output_acceleration: out signed(15 downto 0) := (others => '0')
        );
end speed_control;


architecture synth of speed_control is
  -- Parts of 16
  constant c_prop: integer := 10;
  constant c_int: integer := 1;
  constant c_diff: integer := 1;

  
  signal e: integer := 0;
  signal e_alt: integer := 0;
  signal esum: integer := 0;
  signal ediff: integer := 0;

  signal clockshifter: unsigned(control_clock_divider downto 0) := (others => '0');
  signal controlclock: std_logic := '0';
begin

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      clockshifter <= clockshifter + 1;
    end if;
  end process;


  controlclock <= clockshifter(clockshifter'left);

  process(controlclock)
    variable delta: integer := 0;
  begin
    if rising_edge(controlclock) then
      delta := to_integer(desired_speed) - to_integer(speed_front);
      esum <= esum + delta;
      ediff <= delta - e;
      --output_acceleration <= shift_right(to_signed((c_prop * delta + c_int * esum + c_diff * ediff), 32),4);
      --fixme: output_acceleration war vorher 32 bit. wieso? ausgabe sollte eigtl 16 bit sein
      output_acceleration <= (others => '0');
      e <= delta;
    end if;
  end process;
  
end synth;

