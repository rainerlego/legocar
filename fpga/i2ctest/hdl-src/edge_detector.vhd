library IEEE;
use ieee.std_logic_1164.all;

entity edge_detector is
  port (clock: in std_logic;
        input: in std_logic;
        output: out std_logic);
end edge_detector;


architecture rtl of edge_detector is
  signal det_reg: std_logic_vector(1 downto 0) := (others => '0');

begin

  process(clock)
  begin
    if rising_edge(clock) then
      det_reg <= det_reg(det_reg'low) & input;
    end if;
  end process;

  output <= not(det_reg(det_reg'high)) and (det_reg(det_reg'low));
end  rtl;

