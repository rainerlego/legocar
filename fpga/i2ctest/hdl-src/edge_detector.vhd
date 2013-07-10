library IEEE;
use ieee.std_logic_1164.all;


-- A is the input, the detector outputs a one-CLK-period pulse on R if A has a
-- rising edge, and a pulse on F if A has a falling edge.
entity edge_detector is
  port (CLK: in std_logic;
        A: in std_logic;
        R: out std_logic;
        F: out std_logic);
end edge_detector;


architecture rtl of edge_detector is
  signal det_reg: std_logic_vector(1 downto 0) := (others => '0');

begin

  process(CLK)
  begin
    if rising_edge(CLK) then
      det_reg <= det_reg(det_reg'low) & A;
    end if;
  end process;

  R <= not(det_reg(det_reg'high)) and (det_reg(det_reg'low));
  F <= det_reg(det_reg'high) and not (det_reg(det_reg'low));
end  rtl;

