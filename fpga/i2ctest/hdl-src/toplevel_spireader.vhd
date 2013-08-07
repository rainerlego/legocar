library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity toplevel_spireader is
  port (
    KEY: in std_logic_vector(3 downto 0);
    LEDR: out std_logic_vector(15 downto 0);
    CLOCK_50: in std_logic;
    HEX: out std_logic_vector(27 downto 0);
    SPI_CLK: in std_logic;
    SPI_CS: in std_logic;
    SPI_MOSI: in std_logic;
    SPI_MISO: out std_logic
    );

end toplevel_spireader;

architecture synth of toplevel_spireader is

  component spireader is
    port (clk_50: in std_logic;
          spiclk: in std_logic;
          spics: in std_logic;
          spimosi: in std_logic;
          spimiso: out std_logic;
          led: out std_logic_vector(15 downto 0);
          steering: out unsigned(15 downto 0);       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
          acc: out unsigned(15 downto 0)       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)

          --steering
          --acc
          --speed
          --speed/acc-switch
          );
  end component spireader;

  component seven_segment is
    port (
      number : in  unsigned(3 downto 0);
      output : out std_logic_vector(6 downto 0));
  end component seven_segment;

  signal testval: unsigned(15 downto 0) := "1111000011110000";
  signal waitcycles: integer;
  
begin
  DEBUG_SEGMENTS: for i in 1 to 4 generate
    SEG: seven_segment port map(number => testval((i * 4 - 1) downto (i - 1) * 4),
                                output => HEX(i * 7 - 1 downto (i - 1) * 7));
    end generate DEBUG_SEGMENTS;

  spireaderi: spireader
    port map(
      clk_50 => CLOCK_50,
      spiclk => SPI_CLK,
      spics => SPI_CS,
      spimosi => SPI_MOSI,
      spimiso => SPI_MISO,
      led => LEDR,
      steering => testval );

end synth;
