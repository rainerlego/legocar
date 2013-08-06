
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity toplevel_spitest is
  generic (
    debounce_cycles: integer := 25_000_000);
  port (
    KEY: in std_logic_vector(3 downto 0);
    --LEDG: inout std_logic_vector(8 downto 0) := (others => '0');
    CLOCK_50: in std_logic;
    HEX: out std_logic_vector(27 downto 0);
    clk: in std_logic;
    cs: in std_logic;
    mosi: in std_logic;
    miso: out std_logic);
    --ext_data_receive: out std_logic_vector(7 downto 0);
    --ext_data_write: in std_logic_vector(7 downto 0);
    --ext_event: out std_logic :='0');
end toplevel_spitest;

architecture synth of toplevel_spitest is

  component spislave is
    port (clk_50: in std_logic;
          clk: in std_logic;
          cs: in std_logic;
          mosi: in std_logic;
          miso: out std_logic;
          ext_data_receive: out std_logic_vector(7 downto 0);
          ext_data_write: in std_logic_vector(7 downto 0) := "11111000";
          ext_event: out std_logic :='0');
  end component;

  component seven_segment is
    port (
      number : in  unsigned(3 downto 0);
      output : out std_logic_vector(6 downto 0));
  end component seven_segment;

  signal start: std_logic := '0';
  signal running: std_logic;
  signal motor: std_logic_vector(0 to 2) := (others => '0');
  signal speed: unsigned(0 to 15) := (others => '0');
  signal waitcycles: integer := 0;
  signal ext_event: std_logic :='0';
  signal ext_data_write: std_logic_vector(7 downto 0) := "11111000";
  signal ext_data_receive: std_logic_vector(7 downto 0);
  --signal testhex: std_logic_vector(7 downto 0) := "11110000";

begin

  spi: spislave
    port map (
      clk_50 => CLOCK_50,
      clk => clk,
      cs => cs,
      mosi => mosi,
      ext_event => ext_event,
      miso => miso,
      ext_data_write => ext_data_write,
      ext_data_receive => ext_data_receive);

  DEBUG_SEGMENTS: for i in 1 to 2 generate
    SEG: seven_segment port map(number => unsigned(ext_data_receive((i * 4 - 1) downto (i - 1) * 4)),
                                output => HEX(i * 7 - 1 downto (i - 1) * 7));
    end generate DEBUG_SEGMENTS;



  process(CLOCK_50)
  begin
    if rising_edge(CLOCK_50) then
      if rising_edge(ext_event) then
      end if;
      if waitcycles > 0 then
        waitcycles <= waitcycles - 1;
      else 
        if KEY(0) = '0' then
          waitcycles <= debounce_cycles;
          ext_data_write <= "00001111";
        elsif KEY(1) = '0' then
          waitcycles <= debounce_cycles;
          ext_data_write <= "10101010";
        elsif KEY(2) = '0' then
          waitcycles <= debounce_cycles;
          ext_data_write <= "11101110";
        end if;
      end if;
    end if;
  end process;
end synth;
