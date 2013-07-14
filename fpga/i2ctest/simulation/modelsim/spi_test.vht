LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_logic_arith.all;

ENTITY spi_test IS
END spi_test;

ARCHITECTURE impl OF spi_test IS

  COMPONENT spislave is
    port (clk: in std_logic;
          cs: in std_logic;
          mosi: in std_logic;
          miso: out std_logic;
          write_buffer: in std_logic_vector(7 downto 0);
          read_buffer: out std_logic_vector(7 downto 0);
          complete: inout std_logic);
  END COMPONENT;

  SIGNAL amclk : std_logic := '0';
  SIGNAL amcs : std_logic := '1';
  SIGNAL ammosi : std_logic := '0';
  SIGNAL ammiso : std_logic := '0';
  SIGNAL amwrite_buffer : std_logic_vector(7 downto 0) := "10110101";
  SIGNAL amread_buffer : std_logic_vector(7 downto 0) := "00000000";


  signal recv: std_logic_vector(7 downto 0) := (others => '0');  signal complete: std_logic := '0';
  signal towrite: std_logic_vector(15 downto 0) := "0111011101101101";
BEGIN
  i1 : spislave
    PORT MAP (
      clk => amclk,
      cs => amcs,
      mosi => ammosi,
      miso => ammiso,
      write_buffer => amwrite_buffer,
      read_buffer => amread_buffer,
      complete => complete);

  PROCESS                                         
  BEGIN
    amcs <= '0';
    for i in 0 to 15 loop
      ammosi <= towrite(i);
      wait for 20 ns;
    end loop;
    WAIT;
  END PROCESS;


  process
  begin
    for i in 0 to 100 loop
      amclk <= '1';
      wait for 10 ns;
      amclk <= '0';
      wait for 10 ns;
    end loop;
    wait;
  end process;  

END impl;
