library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity spireader is
  port (clk_50: in std_logic;
        spiclk: in std_logic;
        spics: in std_logic;
        spimosi: in std_logic;
        speed_front: in unsigned(7 downto 0);
        speed_back: in unsigned(7 downto 0);
        output_acceleration: in unsigned(15 downto 0);
        spimiso: out std_logic;
        led: out std_logic_vector(7 downto 0);
        steering: out unsigned(15 downto 0) := to_unsigned(4000,16);  --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
        acc: out unsigned(15 downto 0) := to_unsigned(4000,16);  --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
        speed: out unsigned(7 downto 0);
        speed_instead_acc: out std_logic := '0';
        enable_antischlupf: out std_logic := '0';
        debugpin: out std_logic
      );
end spireader;

architecture spireaderarch of spireader is
  signal spislave_event: std_logic := '0';
  signal spislave_data_write: std_logic_vector(7 downto 0) := "11111000";
  signal spislave_data_receive: std_logic_vector(7 downto 0);
  signal debugpins: std_logic := '0';
  signal output_acceleration_tmp: unsigned(15 downto 0);
  signal speed_desired: unsigned(7 downto 0);

  type machine is (reset,afterpreamble,cmd_servo,cmd_servo2,cmd_speedraw, get_speed_back, get_output_acceleration1, get_output_acceleration2, get_last_byte1, get_last_byte2, get_speed_desired);
  signal state: machine := reset;

  component spislave is
    port (clk_50: in std_logic;
          clk: in std_logic;
          cs: in std_logic;
          mosi: in std_logic;
          miso: out std_logic;
          ext_data_receive: out std_logic_vector(7 downto 0);
          ext_data_write: in std_logic_vector(7 downto 0);
          ext_event: out std_logic;
          debug: out std_logic);
  end component;

begin

  spi: spislave
    port map (
      clk_50 => clk_50,
      clk => spiclk,
      cs => spics,
      mosi => spimosi,
      miso => spimiso,
      ext_event => spislave_event,
      ext_data_write => spislave_data_write,
      ext_data_receive => spislave_data_receive
      --debug => debugpin);
      );
    
  process(clk_50)
  variable servoval: std_logic_vector(15 downto 0);
  variable servoid: std_logic_vector(3 downto 0);
  begin
    if rising_edge(clk_50) then
      if spislave_event = '1' then
        if debugpins = '0' then
          debugpins <= '1';
        else
          debugpins <= '0';
        end if;

        case state is
          when reset =>
            if spislave_data_receive = "11111111" then
              state <= afterpreamble;
              led(7 downto 0) <= (others => '0');
              led(0) <= '1';
            end if;
          when get_speed_back =>
            spislave_data_write <= std_logic_vector(speed_back);
            state <= get_speed_desired;
            output_acceleration_tmp <= output_acceleration;
          when get_speed_desired =>
            spislave_data_write <= std_logic_vector(speed_desired);
            state <= get_output_acceleration1;
            output_acceleration_tmp <= output_acceleration;
          when get_output_acceleration1 =>
            spislave_data_write <= std_logic_vector(output_acceleration_tmp(15 downto 8));
            state <= get_output_acceleration2;
          when get_output_acceleration2 =>
            spislave_data_write <= std_logic_vector(output_acceleration_tmp(7 downto 0));
            state <= get_last_byte1;
          when get_last_byte1 =>
            state <= get_last_byte2;
          when get_last_byte2 =>
            state <= reset;

          when afterpreamble =>
            case spislave_data_receive(7 downto 4) is
              when "0000" => --servo
                state <= cmd_servo;
                servoid := spislave_data_receive(3 downto 0);
                led(1) <= '1';
              when "1011" => --speedraw
                state <= cmd_speedraw;
                led(1) <= '1';
              when "1001" => --speed/acc switch
                if spislave_data_receive(0) = '1' then
                  speed_instead_acc <= '1';
                  led(6) <= '1';
                else
                  speed_instead_acc <= '0';
                  led(6) <= '0';
                end if;
                state <= reset;
              when "1010" => --antischlupf switch
                if spislave_data_receive(0) = '1' then
                  enable_antischlupf <= '1';
                  led(7) <= '1';
                else
                  enable_antischlupf <= '0';
                  led(7) <= '0';
                end if;
                state <= reset;
              when "1100" => --get speeds, has to be followed by 4 dummy bytes
                spislave_data_write <= std_logic_vector(speed_front);
                state <= get_speed_back;
              when others =>
                state <= reset;
            end case;

      ------------------servo-----------------------------------------
          when cmd_servo =>
            if spislave_data_receive = "11111111" then
              state <= reset;
            else
              state <= cmd_servo2;
              servoval(15 downto 8) := spislave_data_receive;
              led(2) <= '1';
            end if;

          when cmd_servo2 =>
            if spislave_data_receive = "11111111" then
              state <= reset;
            else
              state <= reset;
              servoval(7 downto 0) := spislave_data_receive;
              led(3) <= '1';
              if servoid = "0000" then
                acc <= unsigned(servoval);
              elsif servoid = "0001" then
                steering <= unsigned(servoval);
                led(4) <= '1';
              end if;
            end if;

      ------------------speed-----------------------------------------
          when cmd_speedraw =>
            if spislave_data_receive = "11111111" then
              state <= reset;
            else
              state <= reset;
              speed_desired <= unsigned(spislave_data_receive);
              led(2) <= '1';
            end if;

          when others =>
        end case;
        --steering(7 downto 0) <= "11110000";
        --steering(7 downto 0) <= unsigned(spislave_data_receive);
      end if;
    end if;

  end process;

  debugpin <= debugpins;
  speed <= speed_desired;

end spireaderarch;
