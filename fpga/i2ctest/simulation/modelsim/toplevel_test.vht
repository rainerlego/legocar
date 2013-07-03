library IEEE;
use ieee.std_logic_1164.all;

entity toplevel_test is
end toplevel_test;

architecture simulation of toplevel_test is

  component toplevel is
    generic (
      debounce_cycles: integer := 25_000_000);
    port (
      KEY: in std_logic_vector(3 downto 0);
      LEDG: out std_logic_vector(8 downto 0);
      i2c_scl: inout std_logic := '0';
      i2c_sda: inout std_logic := '0';
      CLOCK_50: in std_logic);
  end component;

  signal KEY: std_logic_vector(3 downto 0) := (others => '0');
  signal LEDG: std_logic_vector(8 downto 0) := (others => '0');
  signal i2c_scl : std_logic := '0';
  signal i2c_sda : std_logic := '0';
  signal CLOCK_50 : std_logic := '0';

begin
  
  i1: toplevel generic map (debounce_cycles => 1000) port map (KEY => KEY, LEDG => LEDG, i2c_scl => i2c_scl, i2c_sda => i2c_sda, CLOCK_50 => CLOCK_50);

  process
  begin
    for i in 0 to  110_000 loop
      CLOCK_50 <= not CLOCK_50;
      wait for 10 ns;
    end loop;
    wait;
  end process;

  process
  begin
    KEY <= "1111";
    wait for 20 ns;
    KEY <= "1110";
    wait for 20 ns;
    KEY <= "1111";
    wait for 550000 ns;
    KEY <= "1101";
    wait for 20 ns;
    KEY <= "1111";
    wait for 550000 ns;
    KEY <= "1011";
    wait for 20 ns;
    KEY <= "1111";
    wait;
  end process;
end simulation;
