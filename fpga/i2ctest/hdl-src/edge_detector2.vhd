
library IEEE;
use ieee.std_logic_1164.all;


-- A is the input, the detector outputs a one-CLK-period pulse on R if A has a
-- rising edge, and a pulse on F if A has a falling edge.
entity edge_detector2 is
  port (CLK: in std_logic;
        A: in std_logic;
        R: out std_logic;
        F: out std_logic);
end edge_detector2;


architecture rtl of edge_detector2 is
  signal det_reg: std_logic_vector(1 downto 0) := (others => '0');
  signal smoother: std_logic_vector(15 downto 0) := (others=> '0');

begin

  process(CLK)
  begin
    if rising_edge(CLK) then
      smoother(15 downto 1) <= smoother(14 downto 0);
      if    smoother = "0000000000000000" then
        det_reg <= det_reg(det_reg'low) & '0';
      elsif smoother = "1111111111111111" then
        det_reg <= det_reg(det_reg'low) & '1';
      end if;
    end if;
  end process;
  smoother(0) <= A;

  R <= not(det_reg(det_reg'high)) and (det_reg(det_reg'low));
  F <= det_reg(det_reg'high) and not (det_reg(det_reg'low));
end  rtl;
