library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity speed_control is
  generic (clock_divider: integer := 5000000); --1/10 sec
  port (CLOCK_50: in std_logic;
        speed_front: in unsigned(7 downto 0);
        speed_back: in unsigned(7 downto 0);
        desired_speed: in unsigned(7 downto 0);
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

  signal clockcount: integer := 0;
  signal clockshifter: unsigned(control_clock_divider downto 0) := (others => '0');
  signal controlclock: std_logic := '0';
begin


  process(CLOCK_50)
    variable delta: integer := 0;
    variable tmp: integer := 0;
  begin
    if rising_edge(CLOCK_50) then

      clockcount <= clockcount + 1;
      if clockcount >= clock_divider then
        clockcount <= 0;
        edgecount <= 0;
        

        delta := to_integer(desired_speed) - to_integer(speed_front);
        esum <= esum + delta;
        --ediff <= delta - e;
      end if

      tmp = c_prop * delta + c_int * esum;
      if tmp < 4000 then
        output_acceleration <= 4000 + c_prop * delta + c_int * esum;
      else
        output_acceleration <= 4000;
      end if;

      
    end if;
  end process;
  
end synth;

