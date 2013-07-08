library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity toplevel is
  generic (
    debounce_cycles: integer := 25_000_000);
  port (
    KEY: in std_logic_vector(3 downto 0);
    LEDG: out std_logic_vector(8 downto 0) := (others => '0');
    i2c_scl: inout std_logic := '0';
    i2c_sda: inout std_logic := '0';
    CLOCK_50: in std_logic);
end toplevel;

architecture synth of toplevel is
  component motor_controller is
    generic (slave_address: std_logic_vector(6 downto 0));
    port (i2c_scl: inout std_logic;
          i2c_sda: inout std_logic;
          CLOCK_50: in std_logic;
          start: in std_logic;
          running: out std_logic;
          motor: in std_logic_vector(0 to 2);
          -- Only allow 2 byte arguments
          speed: in unsigned(0 to 15));
  end component;


  signal start: std_logic := '0';
  signal running: std_logic;
  signal motor: std_logic_vector(0 to 2) := (others => '0');
  signal speed: unsigned(0 to 15) := (others => '0');
  signal waitcycles: integer := 0;

begin

  controller: motor_controller
    generic map (
      slave_address => "0000011"
      )
    port map (
      CLOCK_50 => CLOCK_50,
      start => start,
      running => running,
      motor => motor,
      speed => speed,
      i2c_scl => i2c_scl,
      i2c_sda => i2c_sda);

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if running = '0' then
        if waitcycles > 0 then
          waitcycles <= waitcycles - 1;
        else 
          if KEY(0) = '0' then
            speed <= (others => '0');
            motor <= "001";
            start <= '1';
            LEDG(0) <= '1';
            waitcycles <= debounce_cycles;
          elsif KEY(1) = '0' then
            speed <= conv_unsigned(2000, 16);
            motor <= "001";
            start <= '1';
            LEDG(1) <= '1';
            waitcycles <= debounce_cycles;
          elsif KEY(2) = '0' then
            speed <= conv_unsigned(4000, 16);
            motor <= "001";
            start <= '1';
            LEDG(2) <= '1';
            waitcycles <= debounce_cycles;
          end if;
        end if;
      else
        start <= '0';
      end if;
    end if;
  end process;
end synth;
