library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity seven_segment is
  port (number: in unsigned(3 downto 0);
        output: out std_logic_vector(6 downto 0));
end seven_segment;

architecture synth of seven_segment is
begin
  process(number)
  begin
    case number is
      -- 0
      when "0000" => output <= (6 => '1', others => '0');
      -- 1
      when "0001" => output <= (1 => '0', 2 => '0', others => '1');
      -- 2
      when "0010" => output <= (2 => '1', 5 => '1', others => '0');
      -- 3
      when "0011" => output <= (4 => '1', 5 => '1', others => '0');
      -- 4
      when "0100" => output <= (0 => '1', 4 => '1', 3 => '1', others => '0');
      -- 5
      when "0101" => output <= (1 => '1', 4 => '1', others => '0');
      -- 6
      when "0110" => output <= (1 => '1', others => '0');
      -- 7
      when "0111" => output <= (0 => '0', 1 => '0', 2 => '0', others => '1');
      -- 8
      when "1000" => output <= (others => '0');
      -- 9
      when "1001" => output <= (4 => '1', 3 => '0', others => '0');
      -- A
      when "1010" => output <= (3 => '1', others => '0');
      -- B
      when "1011" => output <= (0 => '1', 1 => '1', others => '0');
      -- C
      when "1100" => output <= (1 => '1', 2 => '1', 6 => '1', others => '0');
      -- D
      when "1101" => output <= (0 => '1', 5 => '1', others => '0');
      -- E
      when "1110" => output <= (1 => '1', 2 => '1', others => '0');
      -- F
      when "1111" => output <= (1 => '1', 2 => '1', 3 => '1', others => '0');
    end case;
  end process;
end synth;
                                    
