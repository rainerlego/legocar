
LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY i2c_master_tst IS
END i2c_master_tst;

ARCHITECTURE i2c_master_tst_arch OF i2c_master_tst IS
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
	
	signal clk: std_logic := '0';
	signal reset_n: std_logic := '0';
	signal ena: std_logic := '0';
	signal addr: std_logic_vector(6 downto 0) := (1 => '1', others => '0');
	signal rw: std_logic := '0';
	signal data_wr: std_logic_vector(7 downto 0) := (others => '0');
	signal busy: std_logic;
	signal data_rd: std_logic_vector(7 downto 0);
	signal ack_error: std_logic;
	signal sda: std_logic;
	signal scl: std_logic;
	
	constant max_cycles: integer := 10_000;
	
BEGIN
	i1 : i2c_master
	PORT MAP (
-- list connections between master ports and signals
		clk => clk,
		reset_n => reset_n,
		ena => ena,
		addr => addr,
		rw => rw,
		data_wr => data_wr,
		busy => busy,
		data_rd => data_rd,
		ack_error => ack_error,
		sda => sda,
		scl => scl
	);

	
init: PROCESS
BEGIN
	for i in 1 to max_cycles loop
		clk <= not clk;
		wait for 1 ns;
	end loop;
WAIT;                                                       
END PROCESS init;

                                        
always : PROCESS                                  
BEGIN                                                         
	ena <= '1';
	reset_n <= '1';
	wait until busy = '0';
	data_wr <= (others => '1');
	wait until busy = '0';
	data_wr <= (1 => '1', 3 => '1', 5 => '1', others => '0');
	wait until busy = '0';
	ena <= '0';
WAIT;
END PROCESS always;                                          
END i2c_master_tst_arch;