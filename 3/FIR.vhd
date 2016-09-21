library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;


entity fir is 
	port( clk   : in  std_logic;
			x	: in  std_logic_vector(15 downto 0);
			y	: out std_logic_vector(33 downto 0));
end fir;

architecture fir_arch of fir is

	constant B0 : integer := 5;
	constant B1 : integer := 6;
	constant B2 : integer := -2;
	 
	signal x0, x1, x2 : integer;

	begin
	
	GET_VALUE: process (clk)
		begin
		if (clk'event and clk='1') then
            x2 <= x1;
            x1 <= x0;
			x0 <= to_integer(signed(x));
			y <= std_logic_vector(to_signed(((B0 * x0)+(B1 * x1)+(B2 * x2)), 34));	
		 end if;
	end process;	

end architecture;