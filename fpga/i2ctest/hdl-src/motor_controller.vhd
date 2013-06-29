library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity motor_controller is
  generic (
    slave_address: std_logic_vector(6 downto 0) := (others => '0'));
  port (i2c_scl: inout std_logic;
        i2c_sda: inout std_logic;
        CLOCK_50: in std_logic;
        start: in std_logic;
        running: out std_logic;
        motor: in std_logic_vector(0 to 2);
        -- Only allow 2 byte arguments
        speed: in unsigned(0 to 15));
end motor_controller;


architecture synth of motor_controller is
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

  component edge_detector
    port (clock: in std_logic;
          input: in std_logic;
          output: out std_logic);
  end component;        
  
  type machine is (reset,writing);
  signal addr: std_logic_vector(6 downto 0) := slave_address;
  signal data_wr: std_logic_vector(7 downto 0) := (others => '0');
  signal reset_n: std_logic := '1';
  signal ena: std_logic := '0';
  signal rw: std_logic := '0';
  signal busy: std_logic;
  signal data_rd: std_logic_vector(7 downto 0);
  signal ack_error: std_logic;
  signal state: machine := reset;
  signal busy_count: integer := 0;
  signal busy_edge: std_logic := '0';
begin
  i2c_servoboard: i2c_master
    generic map (bus_clk => 100_000)
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

  busy_detector: edge_detector
    port map (
      clock => CLOCK_50,
      input => busy,
      output => busy_edge);
  
  process(CLOCK_50)
  begin
    -- TODO: Actually use ack_error.
    if rising_edge(CLOCK_50) then
      case state is
        -- Reset state. Keep i2c_master inactive too.
        when reset =>
          if start = '1' then
            state <= writing;
            running <= '1';
          else
            running <= '0';
          end if;
        -- Send the preamble (ff)
        when writing =>
          if busy_edge = '1' then
            busy_count <= busy_count + 1;
          end if;
          case busy_count is
            when 0 =>
              -- Write preamble
              data_wr <= to_stdlogicvector(bit_vector'(x"FF"));
              ena <= '1';
            when 1 =>
              -- Send command
              data_wr <= to_stdlogicvector(bit_vector'(x"0")) & '0' & motor;
            when 2 =>
              data_wr <= conv_std_logic_vector(speed(0 to 7), 8);
            when 3 =>
              data_wr <= conv_std_logic_vector(speed(8 to 15), 8);
            when others =>
              state <= reset;
              running <= '0';
              busy_count <= 0;
              ena <= '0';
          end case;
      end case;
    end if;
  end process;
end synth;
