library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

entity spislave is
  port (clk: in std_logic;
        cs: in std_logic;
        mosi: in std_logic;
        miso: out std_logic;
        write_buffer: in std_logic_vector(7 downto 0);
        read_buffer: out std_logic_vector(7 downto 0);
        data: inout std_logic_vector(8 downto 0));
end spislave;

architecture spislavearch of spislave is
begin
  process(clk) --clock idle low -> write data on rising edge
    variable count : integer range 0 to 10 :=7;  --start value 7: write-buffer will be written to data
  begin
    if cs = '0' then
      if clk = '0' then --falling edge -> toggle
        data(8 downto 1) <= data(7 downto 0);  -- shift left
        count:=count+1;
          case count is
            when 6 => --beim 7. mal shiften: ein komplettes byte wurde empfangen, letztes bit wird ncoh gesendet
              read_buffer <= data(7 downto 0);
            when 7 => --beim 8. mal shiften: letztes bit wurde gesendet, empfangene daten wurden in read_buffer geschrieben
              data(8 downto 1) <= write_buffer;
              count := 0;
          end case;
      else              --rising edge -> sample
        data(0) <= mosi;
      end if;

    end if;
  end process;
  miso <= data(8);
end spislavearch;
