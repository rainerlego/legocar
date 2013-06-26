library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

entity spislave is
  port (clk: in std_logic;
        cs: in std_logic;
        mosi: in std_logic;
        miso: out std_logic;
        complete: inout std_logic;
        data: inout std_logic_vector(8 downto 0);)
end spislave;

architecture spislavearch of spislave is
begin
  process(clk) --clock idle low -> write data on rising edge
  begin
    if cs = '0' then
      if clk = '0' then --falling edge -> toggle
        data <= data sll 1;
      else              --rising edge -> sample
        data(0) <= mosi;
      end if;
    end if;
  end process;
  miso <= data(7);
end spislavearch;
