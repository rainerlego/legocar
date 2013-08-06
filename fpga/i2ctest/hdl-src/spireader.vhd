library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity spireader is
  port (clk_50: in std_logic;
        spiclk: in std_logic;
        spics: in std_logic;
        spimosi: in std_logic;
        spimiso: out std_logic;
        steering: out unsigned(15 downto 0);       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
        acc: out unsigned(15 downto 0)       --desired servo-postition/motor-acceleration (0 - 4000 - 8000)
      );

        --steering
        --acc
        --speed
        --speed/acc-switch
end spireader;

architecture spireaderarch of spireader is
  signal spislave_event: std_logic := '0';
  signal spislave_data_write: std_logic_vector(7 downto 0) := "11111000";
  signal spislave_data_receive: std_logic_vector(7 downto 0);

  signal servoid: std_logic_vector(3 downto 0);
  signal servoval: std_logic_vector(15 downto 0);

  type machine is (reset,cmd_servo,cmd_servo2);
  signal state: machine := reset;

  component spislave is
    port (clk_50: in std_logic;
          clk: in std_logic;
          cs: in std_logic;
          mosi: in std_logic;
          miso: out std_logic;
          ext_data_receive: out std_logic_vector(7 downto 0);
          ext_data_write: in std_logic_vector(7 downto 0);
          ext_event: out std_logic);
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
      ext_data_receive => spislave_data_receive);
    
  process(clk_50)
  begin
    if rising_edge(clk_50) then
      if spislave_event = '1' then
        case state is
          when reset =>
            case spislave_data_read(7 downto 4) is
              when "0000" => --servo
                state <= cmd_servo;
                servoid <= spislave_data_read(3 downto 0);
              when others =>
                state <= reset;
            end case;

          when cmd_servo =>
            state <= cmd_servo2;
            servoval(15 downto 8) <= spislave_data_read;

          when cmd_servo2 =>
            state <= reset;
            servoval(7 downto 0) <= spislave_data_read;
            if servoid = "0000" then
              acc <= unsigned(servoval);
            elsif servoid = "0001" then
              steering <= unsigned(servoval);
            end if;
          when others =>
        end case;
        --steering(7 downto 0) <= "11110000";
        --steering(7 downto 0) <= unsigned(spislave_data_receive);
      end if;
    end if;

  end process;
  
end spireaderarch;
