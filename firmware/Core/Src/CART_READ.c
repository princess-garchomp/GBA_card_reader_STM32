#include "CART_READ.h"

uint16_t read_rom_word(uint32_t input)
{
	uint16_t return_value;
	set_cart_rom_pins_output();
	Delay(1);

	// Divide address by two to get word addressing
	input = input >> 1;

	write_rom_address(input);
	Delay(1);
	CS_LOW();
	Delay(1);
	set_cart_rom_pins_input();
	Delay(1);
	RD_LOW();
	Delay(1);
	return_value = read_cart_rom();
	CS_HIGH();
	RD_HIGH();
	Delay(1);

	return return_value;



}
