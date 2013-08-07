library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity spislave is
  port (clk_50: in std_logic;
        clk: in std_logic;
        cs: in std_logic;
        mosi: in std_logic;
        miso: out std_logic;
        ext_data_receive: out std_logic_vector(7 downto 0);
        ext_data_write: in std_logic_vector(7 downto 0);
        ext_event: out std_logic :='0');
end spislave;

architecture spislavearch of spislave is
  signal count : integer range 0 to 10 := 0;  --start value 7: write-buffer will be written to data
  signal data: std_logic_vector(8 downto 0) := (others => '0');
  signal write_buffer: std_logic_vector(7 downto 0);
  signal read_buffer: std_logic_vector(7 downto 0);
  signal complete: std_logic := '0';
  signal clk_r: std_logic := '0';
  signal clk_f: std_logic := '0';
  signal cs_r: std_logic := '0';
  signal cs_f: std_logic := '0';

  component edge_detector is
    port (
      CLK  : in  std_logic;
      A  : in  std_logic;
      R : out std_logic;
      F: out std_logic);
  end component edge_detector;

  

begin
  clk_edge: edge_detector
    port map(CLK => clk_50, A => clk, R => clk_r, F => clk_f);
  cs_edge: edge_detector
    port map(CLK => clk_50, A => cs, R => cs_r,  F => cs_f);
    
  process(clk_50) --clock idle low -> write data on rising edge
  begin
    
    if rising_edge(clk_50) then

      if cs_f = '1' then
        --complete <= '1';
        count <= 0;
      end if;

      if cs = '0' then -- we receive/send data
        if clk_f = '1' then  -- falling edge -> toggle
          data(8 downto 1) <= data(7 downto 0);  -- shift left
          count <= count+1;
          case count is
            when '7' => --beim 8. mal shiften: letztes bit wurde gesendet, empfangene daten wurden in read_buffer geschrieben
              --read_buffer <= data(7 downto 0);
              --data(8 downto 1) <= write_buffer;
              data(8 downto 1) <= ext_data_write;
              complete <= '1';
              count <= 0;
            when others =>
              complete <= '0';
          -- pass.
          end case; 
        elsif clk_r = '1' then -- rising edge -> sample
          data(0) <= mosi;
          if count = 7 then
              ext_data_receive <= data(7 downto 0);
          end if;
        end if;
      end if;


      if complete = '1' then
        complete <= '0';
        --ext_data_receive <= read_buffer;
        --write_buffer <= ext_data_write;
        ext_event <= '1'; -- daten vorher im buffer werden jetzt verschickt
                          -- empfangene daten liegen bereit
      else
        ext_event <= '0';
      end if;

    end if;

  end process;
  miso <= data(8);
  
end spislavearch;
