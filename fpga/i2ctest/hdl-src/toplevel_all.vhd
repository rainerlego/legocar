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
    SPEED_PULSE_FRONT: in std_logic;
    SPEED_PULSE_BACK: in std_logic;
    KEY: in std_logic_vector(3 downto 0);
    --HEX: out std_logic_vector(27 downto 0);
    HEX0: out std_logic_vector(6 downto 0);
    HEX1: out std_logic_vector(6 downto 0);
    HEX2: out std_logic_vector(6 downto 0);
    HEX3: out std_logic_vector(6 downto 0);
    HEX4: out std_logic_vector(6 downto 0);
    HEX5: out std_logic_vector(6 downto 0);
    HEX6: out std_logic_vector(6 downto 0);
    HEX7: out std_logic_vector(6 downto 0);
    DEBUGPIN: out std_logic;
    LEDR: out std_logic_vector(17 downto 0);
    LEDG: out std_logic_vector(7 downto 0)
    );
end toplevel_all;

architecture synth of toplevel_all is

  component spireader is
    port (clk_50: in std_logic;
          spiclk: in std_logic;
          spics: in std_logic;
          spimosi: in std_logic;
          spimiso: out std_logic;
          led: out std_logic_vector(7 downto 0);
          steering: out unsigned(15 downto 0);       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
          acc: out unsigned(15 downto 0);       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
          speed: out unsigned(7 downto 0);
          speed_instead_acc: out std_logic;
          enable_antischlupf: out std_logic;
          debugpin: out std_logic
          );
  end component spireader;

  component speed_sensor2 is
    port (
      clk_in: in std_logic;
      pulse: in std_logic;
      -- in rotations per second
      speed: out unsigned(7 downto 0) := (others => '0'));
  end component;

  component speed_control is
    port (
      CLOCK_50            : in  std_logic;
      enable_antischlupf  : in  std_logic;
      speed_front         : in  unsigned(7 downto 0);
      speed_back          : in  unsigned(7 downto 0);
      desired_speed       : in  unsigned(7 downto 0);
      output_acceleration : out unsigned(15 downto 0) := (others => '0'));
  end component speed_control;

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

  signal speedc_speed_front: unsigned(7 downto 0) := (others => '0');
  signal speedc_speed_back: unsigned(7 downto 0) := (others => '0');
  signal speedc_desired_speed: unsigned(7 downto 0) := (others => '0');
  signal speedc_enable_antischlupf: std_logic := '0';
  signal speedc_acc_out: unsigned(15 downto 0) := (others => '0');

  signal waitcycles: integer;
  signal ledi2ctransmission: std_logic := '0';
  signal debugpins: std_logic := '0';
  
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
      led => LEDR(7 downto 0),
      steering => steering,
      acc => ss_acc_in,
      speed => speedc_desired_speed,
      speed_instead_acc => ss_speed_instead_acc,
      enable_antischlupf => speedc_enable_antischlupf,
      debugpin => debugpins );

  sensor_front: speed_sensor2
    port map (clk_in => CLOCK_50, pulse => SPEED_PULSE_FRONT, speed => speedc_speed_front);

  sensor_back: speed_sensor2
    port map (clk_in => CLOCK_50, pulse => SPEED_PULSE_BACK, speed => speedc_speed_back);

  speed_cont: speed_control
    port map (
        CLOCK_50 => CLOCK_50,
        speed_front => speedc_speed_back,
        speed_back => speedc_speed_front,
        desired_speed => speedc_desired_speed,
        enable_antischlupf => speedc_enable_antischlupf,
        output_acceleration => speedc_acc_out);
    --TODO: speedc_acc_out auf ss_speed_in legen (wird derzeit noch über tasten gesteuert)
    --TODO: speed front und back sind noch vertauscht

  speed_acc_switchi: speed_acc_switch
    port map (
      clk_50 => CLOCK_50,
      acc_out => ss_acc_out,
      acc_in => ss_acc_in,
      speed_in => ss_speed_in,
      speed_instead_acc => ss_speed_instead_acc
      );

  SEG0: seven_segment port map ( number => speedc_acc_out(3 downto 0), output => HEX0);
  SEG1: seven_segment port map ( number => speedc_acc_out(7 downto 4), output => HEX1);
  SEG2: seven_segment port map ( number => speedc_acc_out(11 downto 8), output => HEX2);
  SEG3: seven_segment port map ( number => speedc_acc_out(15 downto 12), output => HEX3);

  SEG4: seven_segment port map ( number => speedc_desired_speed(3 downto 0), output => HEX4);
  SEG5: seven_segment port map ( number => speedc_desired_speed(7 downto 4), output => HEX5);

  SEG6: seven_segment port map ( number => speedc_speed_back(3 downto 0), output => HEX6);
  SEG7: seven_segment port map ( number => speedc_speed_back(7 downto 4), output => HEX7);

  servo0 <= ss_acc_out;
  servo1 <= steering;

  ss_speed_in <= speedc_acc_out;

  DEBUGPIN <= debugpins;

  LEDG(4) <= ledi2ctransmission;
  LEDG(6) <= debugpins;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if ss_speed_instead_acc = '1' then
        LEDG(1 downto 0) <= "10";
      else
        LEDG(1 downto 0) <= "11";
      end if;
    end if;
  end process;

  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if not (waitcycles > 0) then
        -- TODO: Timeout einbauen
        if sc_running = '0' then
          sc_start <= '1';
          waitcycles <= 5_000_000;
          ledi2ctransmission <= not ledi2ctransmission;
        end if;
      else
        waitcycles <= waitcycles - 1;
        sc_start <= '0';
      end if;
    end if;
  end process;

end synth;
