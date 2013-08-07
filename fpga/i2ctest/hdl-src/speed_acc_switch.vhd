library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity speed_acc_switch is
  port (clk_50: in std_logic;
        acc_out: out unsigned(15 downto 0); -- 0 4000 8000
        acc_in: in unsigned(15 downto 0);   -- 0 4000 8000
        speed_in: in unsigned(15 downto 0); -- 0 4000 8000
        speed_instead_acc: in std_logic     -- select: 1: speed, 0: acc
      );
end speed_acc_switch;

architecture speed_acc_switcharch of speed_acc_switch is
begin

  process(clk_50)
  begin
    if rising_edge(clk_50) then
      if speed_instead_acc = '1' then
        acc_out <= speed_in;
      else
        acc_out <= acc_in;
      end if;
    end if;
  end process;

end speed_acc_switcharch;
