#ifndef GPIO_H
#define GPIO_H

#include "stm32f1xx.h"

//the intent with the following functions is to confirm how to conifure the GPIO of this micro
void gpio_pin_one_output_config(void);
void gpio_pin_two_input_config(void);
void set_gpio_pin_one(void);
void reset_gpio_pin_one(void);
_Bool check_gpio_two_input(void);

//the intent of the following functions is to be the drivers for the cart reader

void init_IO();

void set_cart_rom_pins_input();
void set_cart_rom_pins_output();

void write_rom_address(uint32_t input);
uint16_t read_cart_rom(void);

void init_control_pins();
void CLK_HIGH();
void CLK_LOW();

void CS_HIGH();
void CS_LOW();

void WR_HIGH();
void WR_LOW();

void RD_HIGH();
void RD_LOW();



#endif
