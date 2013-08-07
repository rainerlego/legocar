library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity toplevel_servo is
  port (
    KEY: in std_logic_vector(3 downto 0);
    I2C_SCL: inout std_logic;
    I2C_SDA: inout std_logic;
    CLOCK_50: in std_logic;
    HEX: out std_logic_vector(27 downto 0);
    LEDR: out std_logic_vector(15 downto 0)
    );
end toplevel_servo;

architecture synth of toplevel_servo is

  component servo_controller is
    generic (
      slave_address : std_logic_vector(6 downto 0));
    port (
      i2c_scl  : inout std_logic;
      i2c_sda  : inout std_logic;
      CLOCK_50 : in    std_logic;
      start    : in    std_logic;
      running  : out   std_logic;
      servo0: in unsigned(15 downto 0); --servo position
      servo1: in unsigned(15 downto 0) --( 0 - 4000 - 8000 )
      );
  end component servo_controller;

  component seven_segment is
    port (
      number : in  unsigned(3 downto 0);
      output : out std_logic_vector(6 downto 0));
  end component seven_segment;

  signal start: std_logic := '0';
  signal running: std_logic := '0';
  signal servo0: unsigned(15 downto 0) := to_unsigned(6000,16);
  signal servo1: unsigned(15 downto 0) := to_unsigned(2000,16);

  signal waitcycles: integer;
  
begin
  mot_controller: servo_controller
    generic map (slave_address => "0000011")
    port map (
      CLOCK_50 => CLOCK_50,
      start => start,
      running => running,
      servo0 => servo0,
      servo1 => servo1,
      i2c_scl => I2C_SCL,
      i2c_sda => I2C_SDA);

  DEBUG_SEGMENTS: for i in 1 to 2 generate
    SEG: seven_segment port map(number => servo0((i * 4 - 1) downto (i - 1) * 4),
                                output => HEX(i * 7 - 1 downto (i - 1) * 7));
    end generate DEBUG_SEGMENTS;

  DEBUG_SEGMENTS2: for i in 1 to 2 generate
    SEG2: seven_segment port map(number => servo1((i * 4 - 1) downto (i - 1) * 4),
                                output => HEX((i+2) * 7 - 1 downto ((i+2) - 1) * 7));
    end generate DEBUG_SEGMENTS2;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if not (waitcycles > 0) then
        -- TODO: Timeout einbauen
        if running = '0' then
          start <= '1';
          waitcycles <= 5_000_000;
        end if;
      else
        waitcycles <= waitcycles - 1;
        start <= '0';
      end if;
    end if;
  end process;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if KEY(0) = '0' then
        servo0 <= to_unsigned(2000,16);
        LEDR(3 downto 0) <= "0001";
      elsif KEY(1) = '0' then
        servo0 <= to_unsigned(6000,16);
        LEDR(3 downto 0) <= "0010";
      elsif KEY(2) = '0' then
        servo1 <= to_unsigned(2000,16);
        LEDR(3 downto 0) <= "0100";
      elsif KEY(3) = '0' then
        servo1 <= to_unsigned(6000,16);
        LEDR(3 downto 0) <= "1000";
      end if;
    end if;
  end process;

end synth;
