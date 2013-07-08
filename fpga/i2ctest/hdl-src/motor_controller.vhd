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
        motor: in std_logic_vector(2 downto 0);
        -- Only allow 2 byte arguments
        speed: in unsigned(15 downto 0));
end motor_controller;


architecture synth of motor_controller is
  component i2c_master
    GENERIC(
      input_clk : INTEGER := 50_000_000; --input clock speed from user logic in Hz
      bus_clk   : INTEGER := 50_000);   --speed the i2c bus (scl) will run at in Hz
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


  component edge_detector is
    port (
      clock  : in  std_logic;
      input  : in  std_logic;
      output : out std_logic);
  end component edge_detector;

  type machine is (reset,started,preamble,command,arg1,arg2,finished);
  signal addr: std_logic_vector(6 downto 0) := slave_address;
  signal data_wr: std_logic_vector(7 downto 0) := (others => '0');
  signal reset_n: std_logic := '0';
  signal ena: std_logic := '0';
  signal rw: std_logic := '0';
  signal busy: std_logic;
  signal ack_error: std_logic;
  signal state: machine := reset;
  signal busy_pulse: std_logic;

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
      ack_error => ack_error,
      sda => i2c_sda,
      scl => i2c_scl);

  busy_edge: edge_detector
    port map(CLOCK_50, busy, busy_pulse);


  process(CLOCK_50,start, busy)
  begin
    if rising_edge(CLOCK_50) then
      case state is
        when reset =>
          if start = '1' then
            state <= started;
          end if;
        when started =>
          if busy = '0' then
            state <= preamble;
          end if;
        when preamble =>
          if busy_pulse = '1' then
            state <= command;
          end if;
        when command =>
          if busy_pulse = '1' then
            state <= arg1;
          end if;
        when arg1 =>
          if busy_pulse = '1' then
            state <= arg2;
          end if;
        when arg2 =>
          if busy_pulse = '1' then
            state <= finished;
          end if;
        when finished =>
          if busy = '0' then
            state <= reset;
          end if;
      end case;
    end if;
  end process;


  process(state,motor,speed)
  begin
    -- TODO: Actually use ack_error.
    reset_n <= '1';
    ena <= '0';
    running <= '1';
    addr <= (others => '0');
    rw <= '0';
    data_wr <= (others => '0');
    case state is
      when reset =>
        running <= '0';
      when started =>
        addr <= slave_address;
      when preamble =>
        data_wr <= "11111111";
        ena <= '1';
        addr <= slave_address;
      when command =>
        data_wr <= "00000" & motor;
        ena <= '1';
        addr <= slave_address;
      when arg1 => 
        data_wr <= conv_std_logic_vector(speed(7 downto 0), 8);
        ena <= '1';
        addr <= slave_address;
      when arg2 => 
        data_wr <= conv_std_logic_vector(speed(15 downto 8), 8);
        addr <= slave_address;
        ena <= '1';
      when finished =>
        running <= '0';
    end case;
  end process;
end synth;
