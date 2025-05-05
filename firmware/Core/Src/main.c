#include "GPIO.h"
#include "CLOCK.h"
#include "UART.h"
#include "CART_READ.h"
#include "UART_TO_USB_COMM.h"
#include "CRC.h"
#include "configuration.h"

void big_test_foo();

int main(void)
{
	//UGLY_configuration_init();
	gpio_pin_one_output_config();
	gpio_pin_two_input_config();
	init_CRC();

	enable_PLL_sysclock_src_external_HSE_PLL_src();
	config_systic_for_ms(72000);//72000000 would result in 1ms interrutps on the systick. so lets change it to us by removing 3 zeroes
	init_IO();
	set_cart_rom_pins_output();
	config_UART1_blocking_115200baud();

	while (1)
	{
		//UGLY_coniguration_main_foo();
		(void) big_test_foo();

	}
}

big_test_foo()
{
	static uint16_t tempWord=0;
	static uint32_t cartSize = 8192000;
	//static uint32_t cartSize = 8704000;

	for (uint32_t myAddress = 0; myAddress < cartSize; myAddress += 512)
	//for (uint32_t myAddress = 298712; myAddress < cartSize; myAddress += 30)
	{
		(void) UART_BYTE_recieve_blocking(USART1);
		//for (int currWord = 0; currWord < 12; currWord += 2)
		for (int currWord = 0; currWord < 512; currWord += 2)
		{

			tempWord = read_rom_word(myAddress + currWord);
			UART_BYTE_send_blocking(USART1, tempWord);
			tempWord = (tempWord>>8);
			UART_BYTE_send_blocking(USART1, tempWord);
		}
	}

}


