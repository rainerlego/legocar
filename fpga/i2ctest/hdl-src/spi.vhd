library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity spislave is
  port (clk: in std_logic;
        cs: in std_logic;
        mosi: in std_logic;
        miso: out std_logic;
        write_buffer: in std_logic_vector(7 downto 0);
        read_buffer: out std_logic_vector(7 downto 0);
        complete: inout std_logic := '0');
end spislave;

architecture spislavearch of spislave is
  signal count : integer range 0 to 10 := 0;  --start value 7: write-buffer will be written to data
  signal data: std_logic_vector(8 downto 0) := (others => '0');

begin
  process(clk) --clock idle low -> write data on rising edge
  begin
    if cs = '0' then
      if falling_edge(clk) then --falling edge -> toggle
        data(8 downto 1) <= data(7 downto 0);  -- shift left
        count <= count+1;
        case count is
          when 7 => --beim 8. mal shiften: letztes bit wurde gesendet, empfangene daten wurden in read_buffer geschrieben
            read_buffer <= data(7 downto 0);
            data(8 downto 1) <= write_buffer;
            complete <= '1';
            count <= 0;
          when others =>
        -- pass.
        end case;
      elsif rising_edge(clk) then              --rising edge -> sample
        data(0) <= mosi;
      end if;
    end if;
  end process;

  miso <= data(8);
  
end spislavearch;
