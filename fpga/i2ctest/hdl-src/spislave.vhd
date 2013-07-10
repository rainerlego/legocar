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

begin
  process(clk,cs) --clock idle low -> write data on rising edge
  begin
    --complete <= '0';
    --if falling_edge(cs) then
    --  complete <= '1';
    --end if;
    if cs = '0' then
      if falling_edge(clk) then --falling edge -> toggle
        data(8 downto 1) <= data(7 downto 0);  -- shift left
        count <= count+1;
        case count is
          when 7 => --beim 8. mal shiften: letztes bit wurde gesendet, empfangene daten wurden in read_buffer geschrieben
            --read_buffer <= data(7 downto 0);
            --data(8 downto 1) <= write_buffer;
            complete <= '1';
            count <= 0;
          when others =>
            complete <= '0';
        -- pass.
        end case;
      elsif rising_edge(clk) then              --rising edge -> sample
        data(0) <= mosi;
      end if;
    end if;
  end process;

  process(clk_50)
  begin
    if rising_edge(clk_50) then
      if complete = '1' then
        ext_data_receive <= data(7 downto 0);
        data(8 downto 1) <= ext_data_write;
        --ext_data_receive <= read_buffer;
        --write_buffer <= ext_data_write;
        ext_event <= '1';
      else
        ext_event <= '0';
      end if;
    end if;
  end process;

  miso <= data(8);
  
end spislavearch;
