library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

entity i2ctest is
  generic (slave_address: std_logic_vector(6 downto 0) := (others => '0'));
    port (i2c_scl: inout std_logic;
          i2c_sda: inout std_logic;
          CLOCK_50: in std_logic;
          KEY: in std_logic_vector(3 downto 0)
          );
end i2ctest;


architecture simulate of i2ctest is
  component i2c_master
    GENERIC(
      input_clk : INTEGER := 50_000_000; --input clock speed from user logic in Hz
      bus_clk   : INTEGER := 400_000);   --speed the i2c bus (scl) will run at in Hz
    PORT(
      clk       : IN     STD_LOGIC;                    --system clock
      reset_n   : IN     STD_LOGIC;                    --active low reset
      ena       : IN     STD_LOGIC;                    --latch in command
      addr      : IN     STD_LOGIC_VECTOR(6 DOWNTO 0); --address of target slave
      rw        : IN     STD_LOGIC;                    --'0' is write, '1' is read
      data_wr   : IN     STD_LOGIC_VECTOR(7 DOWNTO 0); --data to write to slave
      busy      : OUT    STD_LOGIC;                    --indicates transaction in progress
      data_rd   : OUT    STD_LOGIC_VECTOR(7 DOWNTO 0); --data read from slave
      ack_error : BUFFER STD_LOGIC;                    --flag if improper acknowledge from slave
      sda       : INOUT  STD_LOGIC;                    --serial data output of i2c bus
      scl       : INOUT  STD_LOGIC);                   --serial clock output of i2c bus
  end component;
  
  type machine is (reset,preamble,command,firstbyte,secondbyte,final);
  signal addr: std_logic_vector(6 downto 0) := slave_address;
  signal data_wr: std_logic_vector(7 downto 0) := (others => '0');
  signal reset_n: std_logic := '0';
  signal ena: std_logic := '0';
  signal rw: std_logic := '0';
  signal busy: std_logic;
  signal data_rd: std_logic_vector(7 downto 0);
  signal ack_error: std_logic;
  signal state: machine := reset;
  
begin
  i2c_servoboard: i2c_master
    generic map (bus_clk => 400_000)
    port map(
      clk => CLOCK_50,
      reset_n => reset_n,
      ena => ena,
      addr => addr,
      rw => rw,
      data_wr => data_wr,
      busy => busy,
      data_rd => data_rd,
      ack_error => ack_error,
      sda => i2c_sda,
      scl => i2c_scl);
  
  process(CLOCK_50)
  begin
    -- TODO: Add error transitions.
    if rising_edge(CLOCK_50) then
      case state is
        -- Reset state. Keep i2c_master inactive too.
        when reset =>
          if KEY(0) = '1' then
            state <= preamble;
            reset_n <= '1';
          else
            reset_n <= '0';
            state <= reset;
          end if;
        -- Send the preamble (ff)
        when preamble =>
          data_wr <= (others => '1');
          ena <= '1';
          state <= command;
        when command =>
          -- If Data is not yet written, do nothing and stay in command state.
          -- We have to wait for busy, otherwise the data is not yet written.
          if busy = '0' then
            -- Command: Set Servo 0
            data_wr <= "00000000";
            state <= firstbyte;
          end if;
        when firstbyte =>
          if busy = '0' then
            data_wr <= to_stdlogicvector(bit_vector'x"1f");
            state <= secondbyte;
          end if;
        when secondbyte =>
          if busy = '0' then
            data_wr <= to_stdlogicvector(bit_vector'x"40");
            state <= final;
          end if;
        when final =>
          -- Everything has been written.
          if busy = '0' then
            ena <= '0';
          end if;
      end case;
    end if;
  end process;
end simulate;
