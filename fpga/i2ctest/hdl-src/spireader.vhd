library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity spireader is
  port (clk_50: in std_logic;
        spiclk: in std_logic;
        spics: in std_logic;
        spimosi: in std_logic;
        spimiso: out std_logic;
        steering: out unsigned(15 downto 0);       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
        acc: out unsigned(15 downto 0)       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
			);

				--steering
				--acc
				--speed
				--speed/acc-switch
end spireader;

architecture spireaderarch of spireader is
  signal spislave_event_r: std_logic := '0';
  signal spislave_event_f: std_logic := '0';
  signal spislave_event: std_logic := '0';
  signal spislave_data_write: std_logic_vector(7 downto 0) := "11111000";
  signal spislave_data_receive: std_logic_vector(7 downto 0);

  component edge_detector is
    port (
      CLK  : in  std_logic;
      A  : in  std_logic;
      R : out std_logic;
      F: out std_logic);
  end component edge_detector;

  component spislave is
    port (clk_50: in std_logic;
          clk: in std_logic;
          cs: in std_logic;
          mosi: in std_logic;
          miso: out std_logic;
          ext_data_receive: out std_logic_vector(7 downto 0);
          ext_data_write: in std_logic_vector(7 downto 0);
          ext_event: out std_logic :='0');
  end component;

begin
  spislave_event_detector: edge_detector
    port map(CLK => clk_50, A => spislave_event, R => spislave_event_r, F => spislave_event_f);

  spi: spislave
    port map (
      clk_50 => clk_50,
      clk => spiclk,
      cs => spics,
      mosi => spimosi,
      miso => spimiso,
      ext_event => spislave_event,
      ext_data_write => spislave_data_write,
      ext_data_receive => spislave_data_receive);
    
  process(clk_50)
  begin
    if rising_edge(clk_50) then
      if spislave_event_r = '1' then
				steering(7 downto 0) <= unsigned(spislave_data_receive);
			end if;
    end if;

  end process;
  
end spireaderarch;
