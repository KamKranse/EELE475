
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity GPS_sig is
	port (
		clk 			    	   : in std_logic;
		reset_n 		    	   : in std_logic;
		gps_signal           : in std_logic_vector(31 downto 0);
		avs_s1_write 		   : in std_logic;
		avs_s1_read				: in std_logic;
		avs_s1_address 	   : in std_logic_vector(4 downto 0);
		avs_s1_writedata 	   : in std_logic_vector(31 downto 0);
		avs_s1_readdata      : out std_logic_vector(31 downto 0)
	); 
end GPS_sig;


architecture behavior of GPS_sig is

	signal readdata		 : STD_LOGIC_VECTOR (31 downto 0);
	signal wre : std_logic;  -- write enable
	signal re : std_logic;   -- read  enable
	signal addr : std_logic_vector(4 downto 0);  -- register address
	signal gps_signal2           : std_logic_vector(31 downto 0);
	signal temp  : std_logic_vector(31 downto 0);
	signal temp2 : std_logic_vector(31 downto 0);
	signal gps_output :std_logic_vector(31 downto 0);

begin
	wre  <= avs_s1_write;
	re   <= avs_s1_read;	
	addr <= avs_s1_address;	
	
  process (clk)
	begin
		if rising_edge(clk) and wre='1' and addr="00000" then
			  gps_output <=  avs_s1_writedata;
		end if;
	end process;
	
	process (clk)
	begin
		if rising_edge(clk) and re='1' then
			case addr is
				when "00000" | "10000" => 
					readdata <= gps_signal2;
				when others => readdata <= x"00000000";
			end case;
		end if;
	end process;
	
process(clk)
 begin
 if gps_output < "000000000000000000000000111101" then
	if gps_output < "00000000000000000000000000100000" then
			temp2 <= (gps_output(29 downto 0) & "00"); --multiply by 4
			
			temp <= "1111111111111111111111111" & temp2(6 downto 0); 
	elsif gps_output < "00000000000000000000000000110000" then
			temp2 <= (gps_output(29 downto 0) & "00"); --multiply by 4
			temp <= "00000000000000000000000000" & temp2(5 downto 0); 
	else
			temp2 <= (gps_output(29 downto 0) & "00"); --multiply by 4
			temp <= "0000000000000000000000000" & temp2(6 downto 0); 
	end if;
			gps_signal2 <= temp;
 end if;
end process;
	
	avs_s1_readdata <= readdata;
	
	
	end architecture;