library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity toplevel_all is
  port (
    CLOCK_50: in std_logic;
    I2C_SCL: inout std_logic;
    I2C_SDA: inout std_logic;
    SPI_CLK: in std_logic;
    SPI_CS: in std_logic;
    SPI_MOSI: in std_logic;
    SPI_MISO: out std_logic;
    KEY: in std_logic_vector(3 downto 0);
    HEX: out std_logic_vector(27 downto 0);
    DEBUGPIN: out std_logic;
    LEDR: out std_logic_vector(15 downto 0)
    );
end toplevel_all;

architecture synth of toplevel_all is

  component spireader is
    port (clk_50: in std_logic;
          spiclk: in std_logic;
          spics: in std_logic;
          spimosi: in std_logic;
          spimiso: out std_logic;
          led: out std_logic_vector(15 downto 0);
          steering: out unsigned(15 downto 0);       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
          acc: out unsigned(15 downto 0);       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
          speed_instead_acc: out std_logic;
          debugpin: out std_logic
          );
  end component spireader;

  component speed_acc_switch is
    port (clk_50: in std_logic;
          acc_out: out unsigned(15 downto 0); -- 0 4000 8000
          acc_in: in unsigned(15 downto 0);   -- 0 4000 8000
          speed_in: in unsigned(15 downto 0); -- 0 4000 8000
          speed_instead_acc: in std_logic     -- select: 1: speed, 0: acc
        );
  end component speed_acc_switch;

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

  signal sc_start: std_logic := '0';
  signal sc_running: std_logic := '0';
  signal servo0: unsigned(15 downto 0) := to_unsigned(4000,16);
  signal servo1: unsigned(15 downto 0) := to_unsigned(4000,16);

  signal steering: unsigned(15 downto 0) := to_unsigned(4000,16); -- 0 4000 8000
  
  signal ss_acc_out: unsigned(15 downto 0); -- 0 4000 8000
  signal ss_acc_in: unsigned(15 downto 0) := to_unsigned(4000,16);  -- 0 4000 8000
  signal ss_speed_in: unsigned(15 downto 0) := to_unsigned(4000,16); -- 0 4000 8000
  signal ss_speed_instead_acc: std_logic := '0';

  signal waitcycles: integer;
  
begin
  mot_controller: servo_controller
    generic map (slave_address => "0000011")
    port map (
      CLOCK_50 => CLOCK_50,
      start => sc_start,
      running => sc_running,
      servo0 => servo0,
      servo1 => servo1,
      i2c_scl => I2C_SCL,
      i2c_sda => I2C_SDA);

  spireaderi: spireader
    port map(
      clk_50 => CLOCK_50,
      spiclk => SPI_CLK,
      spics => SPI_CS,
      spimosi => SPI_MOSI,
      spimiso => SPI_MISO,
      led => LEDR,
      steering => steering,
      acc => ss_acc_in,
      speed_instead_acc => ss_speed_instead_acc,
      debugpin => DEBUGPIN );

  speed_acc_switchi: speed_acc_switch
    port map (
      clk_50 => CLOCK_50,
      acc_out => ss_acc_out,
      acc_in => ss_acc_in,
      speed_in => ss_speed_in,
      speed_instead_acc => ss_speed_instead_acc
      );

  DEBUG_SEGMENTS: for i in 1 to 2 generate
    SEG: seven_segment port map(number => ss_acc_out((i * 4 - 1) downto (i - 1) * 4),
                                output => HEX(i * 7 - 1 downto (i - 1) * 7));
    end generate DEBUG_SEGMENTS;

  DEBUG_SEGMENTS2: for i in 1 to 2 generate
    SEG2: seven_segment port map(number => servo1((i * 4 - 1) downto (i - 1) * 4),
                                output => HEX((i+2) * 7 - 1 downto ((i+2) - 1) * 7));
    end generate DEBUG_SEGMENTS2;


  servo0 <= ss_acc_out;
  servo1 <= steering;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if not (waitcycles > 0) then
        -- TODO: Timeout einbauen
        if sc_running = '0' then
          sc_start <= '1';
          waitcycles <= 5_000_000;
        end if;
      else
        waitcycles <= waitcycles - 1;
        sc_start <= '0';
      end if;
    end if;
  end process;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if KEY(0) = '0' then
        ss_speed_in <= to_unsigned(2000,16);
      elsif KEY(1) = '0' then
        ss_speed_in <= to_unsigned(3000,16);
      elsif KEY(2) = '0' then
        ss_speed_in <= to_unsigned(4000,16);
      elsif KEY(3) = '0' then
        ss_speed_in <= to_unsigned(6000,16);
      end if;
    end if;
  end process;

end synth;