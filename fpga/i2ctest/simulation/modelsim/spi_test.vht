LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_logic_arith.all;

ENTITY spi_test IS
END spi_test;

ARCHITECTURE impl OF spi_test IS
  COMPONENT spislave is
    port (mclk: in std_logic;
        mcs: in std_logic;
        mmosi: in std_logic;
        mmiso: out std_logic;
        mwrite_buffer: in std_logic_vector(7 downto 0);
        mread_buffer: out std_logic_vector(7 downto 0);
        mdata: inout std_logic_vector(8 downto 0));
  END COMPONENT;

  SIGNAL amclk : std_logic := '0';
  SIGNAL amcs : std_logic := '1';
  SIGNAL ammosi : std_logic := '0';
  SIGNAL ammiso : std_logic := '0';
  SIGNAL amwrite_buffer : std_logic_vector(7 downto 0) := "10110101";
  SIGNAL amread_buffer : std_logic_vector(7 downto 0) := "00000000";

  SIGNAL CLOCK_50 : STD_LOGIC := '0';
  SIGNAL pulse: STD_LOGIC := '0';
  SIGNAL speed: unsigned(31 downto 0) := (others => '0');
  
BEGIN
  i1 : spislave
    PORT MAP (
      mclk => amclk;
      mcs => amcs;
      mmosi => ammosi;
      mmiso => ammiso;
      mwrite_buffer => amwrite_buffer;
      mread_buffer => amread_buffer;
      );

  PROCESS                                         
  BEGIN
    -- One tenth second simulation.
    ammosi <= '1'; --s1
    wait for 10ns;
    amclk <= '1';
    wait for 10ns;
    amclk <= '0';
    wait for 10ns;
    amclk <= '1'; --first high clock => sample
    recv(7) <= ammiso; --b1
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s2
    wait for 10ns;
    amclk <= '1';
    recv(6) <= ammiso; --b2
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s3
    wait for 10ns;
    amclk <= '1';
    recv(7) <= ammiso; --b3
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s4
    wait for 10ns;
    amclk <= '1';
    recv(6) <= ammiso; --b4
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s5
    wait for 10ns;
    amclk <= '1';
    recv(7) <= ammiso; --b5
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s6
    wait for 10ns;
    amclk <= '1';
    recv(6) <= ammiso; --b6
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s7
    wait for 10ns;
    amclk <= '1';
    recv(7) <= ammiso; --b7
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s8
    wait for 10ns;
    amclk <= '1';
    recv(6) <= ammiso; --b8
    wait for 10ns;
    amclk <= '0';
    ammosi <= '0'; --s9

    WAIT;
  END PROCESS;


END impl;
