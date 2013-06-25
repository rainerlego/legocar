LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY i2ctest_vhd_tst IS
END i2ctest_vhd_tst;

ARCHITECTURE i2ctest_arch OF i2ctest_vhd_tst IS
  SIGNAL CLOCK_50 : STD_LOGIC := '0';
  SIGNAL i2c_scl : STD_LOGIC;
  SIGNAL i2c_sda : STD_LOGIC;
  SIGNAL start : STD_LOGIC;
  SIGNAL motor : STD_LOGIC_VECTOR(2 downto 0) := "011";
  SIGNAL speed : STD_LOGIC_VECTOR(15 downto 0) := x"1f40";
  SIGNAL running: STD_LOGIC;
  COMPONENT i2ctest is
    generic (
      slave_address: std_logic_vector(6 downto 0) := (others => '0'));
    port (i2c_scl: inout std_logic;
          i2c_sda: inout std_logic;
          CLOCK_50: in std_logic;
          motor: in std_logic_vector(0 to 2);
          -- Only allow 2 byte arguments
          speed: in std_logic_vector(0 to 15);
          start: in std_logic;
          running: out std_logic);

  END COMPONENT;
BEGIN
  i1 : i2ctest
    PORT MAP (
      CLOCK_50 => CLOCK_50,
      i2c_scl => i2c_scl,
      i2c_sda => i2c_sda,
      motor => motor,
      speed => speed,
      start => start,
      running => running
      );
  PROCESS                                         
  BEGIN
    for i in 0 to 20000 loop
      CLOCK_50 <= not CLOCK_50;
      wait for 5 ns;
    end loop;
    WAIT;
  END PROCESS;

  PROCESS                                              
  BEGIN
    wait for 20 ns;
    start <= '1';
    wait for 20 ns;
    start <= '0';
    WAIT;                                                        
  END PROCESS;

END i2ctest_arch;
