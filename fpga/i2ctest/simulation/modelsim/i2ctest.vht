LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY i2ctest_vhd_tst IS
END i2ctest_vhd_tst;

ARCHITECTURE i2ctest_arch OF i2ctest_vhd_tst IS
  SIGNAL CLOCK_50 : STD_LOGIC := '0';
  SIGNAL i2c_scl : STD_LOGIC;
  SIGNAL i2c_sda : STD_LOGIC;
  SIGNAL KEY : STD_LOGIC_VECTOR(3 DOWNTO 0);
  COMPONENT i2ctest
    PORT (
      CLOCK_50 : IN STD_LOGIC;
      i2c_scl : INOUT STD_LOGIC;
      i2c_sda : INOUT STD_LOGIC;
      KEY : IN STD_LOGIC_VECTOR(3 DOWNTO 0)
      );
  END COMPONENT;
BEGIN
  i1 : i2ctest
    PORT MAP (
      CLOCK_50 => CLOCK_50,
      i2c_scl => i2c_scl,
      i2c_sda => i2c_sda,
      KEY => KEY
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
    KEY(0) <= '1';
    wait for 20 ns;
    KEY(0) <= '0';
    WAIT;                                                        
  END PROCESS;

END i2ctest_arch;
