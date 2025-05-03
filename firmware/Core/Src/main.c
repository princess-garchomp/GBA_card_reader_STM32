#include "GPIO.h"
#include "CLOCK.h"
#include "UART.h"
#include "CART_READ.h"
#include "UART_TO_USB_COMM.h"
#include "CRC.h"
#include "configuration.h"

int main(void)
{
#if((ROM_INPUT_TESTER==0)&&(ROM_OUTPUT_TESTER==0)&&(TEST_CONTROLL_IO==0)&&(PROOF_OF_CONCEPT==0)&&(PYTHON_TEST_ONE==0))
	gpio_pin_one_output_config();
	gpio_pin_two_input_config();
	init_CRC();
	int i;
	char character;
#endif


#if(HSI_SYSTEM_CLOCK_TEST)//setup the config for testing the HSI as systemclock sournce
	enable_HSI_as_sysclock_src();
	//either of the following config_systick functions will set the systick to fire every 1ms
	//config_systic_for_ms(8000000);
	config_systic(7999);
#endif
#if(HSE_AS_SYSTEM_CLOCK)//setup the config for testing the HSE as systemclock sournce
	enable_HSE_as_sysclock_src();
	//every tick is 1ms
	config_systic_for_ms(8000000);
#endif
#if(HSE_PLL_SYS_CLOCK)
	enable_PLL_sysclock_src_external_HSE_PLL_src();
	config_systic_for_ms(72000000);
#endif
#if(UART_HSE_PLL_BLOCKING|CRC_TEST_ONE)
	enable_PLL_sysclock_src_external_HSE_PLL_src();
	config_systic_for_ms(72000000);
	config_UART1_blocking_9600baud();
#endif
#if(INTERRUPTING_RX_UART|INTERRUPTING_TX_UART)
	enable_PLL_sysclock_src_external_HSE_PLL_src();
	config_systic_for_ms(72000000);
	config_UART1_interrupt(7500);
#endif
#if(UART_DMA_TX_RX_INTERRUPT_TEST)
	enable_PLL_sysclock_src_external_HSE_PLL_src();
	config_systic_for_ms(72000000);
	UART_DMA_TX_RX_INTERRUPT();
#endif
#if(ROM_OUTPUT_TESTER|ROM_INPUT_TESTER|TEST_CONTROLL_IO)
	enable_PLL_sysclock_src_external_HSE_PLL_src();
	config_systic_for_ms(72000000);
	init_IO();
	set_cart_rom_pins_output();
	uint16_t rom_output_tester;
#endif
#if(PROOF_OF_CONCEPT||PYTHON_TEST_ONE)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint16_t read_rom_word(uint32_t input);

	enable_PLL_sysclock_src_external_HSE_PLL_src();
	config_systic_for_ms(72000);//72000000 would result in 1ms interrutps on the systick. so lets change it to us by removing 3 zeroes
	init_IO();
	set_cart_rom_pins_output();
	config_UART1_blocking_115200baud();


	uint16_t tempWord=0;
	//uint32_t cartSize = 800000;
	uint32_t cartSize = 8192000;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

	while (1)
	{
		//the follwing if tests out the GPIO
#if(GPIO_BASIC_TEST)
		//cathoed of led inot pa12, annoed to gnd
		//connecteing pa2 to gnd gets past the while loop (function call returns a value of 0 when connected to gnd)
		set_gpio_pin_one();
		while(check_gpio_two_input());
		for(i=0;i<600000;i++);

		reset_gpio_pin_one();
		while(check_gpio_two_input());
		for(i=0;i<600000;i++);
#endif

		//following if tests the system clock and the dealy fucntion using the HSI as the system clokc source
#if(HSI_SYSTEM_CLOCK_TEST)
		set_gpio_pin_one();
		Delay(1000);//wait for 1000 systick interrupts. 1000ms (1s)
		reset_gpio_pin_one();
		Delay(1000);//wait for 1000 systick interrupts. 1000ms (1s)

#endif
		//following if tests delay using external system clock and pll
#if(HSE_AS_SYSTEM_CLOCK)
		reset_gpio_pin_one();
		Delay(2000);//wait for 2000 systick interrupts. 1000ms (1s)
		set_gpio_pin_one();
		Delay(2000);//wait for 2000 systick interrupts. 1000ms (1s)
#endif
		//the following if uses the HSE as source for PLL
		//the pll is the source for the system clokck
#if(HSE_PLL_SYS_CLOCK)

		Delay(1000);//wait for 1000 systick interrupts. 1000ms (1s)
		set_gpio_pin_one();
		Delay(1000);//wait for 1000 systick interrupts. 1000ms (1s)
		reset_gpio_pin_one();
#endif

#if(UART_HSE_PLL_BLOCKING)
		character = UART_BYTE_recieve_blocking(USART1);
		Delay(500);//wait for 500 systick interrupts. 500ms
		while(check_gpio_two_input());
		UART_BYTE_send_blocking(USART1, character);
		set_gpio_pin_one();
		Delay(500);//wait for 500 systick interrupts. 500ms
		UART_BYTE_send_blocking(USART1, 'B');
		reset_gpio_pin_one();
		Delay(500);//wait for 500 systick interrupts. 500ms
#endif

#if(INTERRUPTING_RX_UART)
		if(get_last_USART_RX() != 0x00)
		{
			UART_BYTE_send_blocking(USART1, get_last_USART_RX());
		}
		else
		{
			UART_BYTE_send_blocking(USART1, 'W');
		}
		while(check_gpio_two_input());
#endif

#if(INTERRUPTING_TX_UART)
		set_USART_TX('A');
		UART_send_byte_interrupt_start(USART1, 'Q');
		Delay(500);
		set_USART_TX('B');
		UART_send_byte_interrupt_start(USART1, 'Q');
		Delay(500);
#endif

#if(CRC_TEST_ONE)
		UART_BYTE_send_blocking(USART1, 'S');//I realize this is only a temporary solution
		UART_BYTE_send_blocking(USART1, 'T');//really need to get it sending arrays of data
		UART_BYTE_send_blocking(USART1, 'A');//this is good enough for noew
		UART_BYTE_send_blocking(USART1, 'R');
		UART_BYTE_send_blocking(USART1, 'T');
		UART_BYTE_send_blocking(USART1, '\r');
		clear_CRC_value();
		UART_BYTE_send_blocking(USART1, get_CRC_data_value());
		UART_BYTE_send_blocking(USART1, '\r');
		Delay(250);
		clear_CRC_value();
		update_CRC_data_value(0xABCD1234);
		update_CRC_data_value(0x1234ABCD);
		UART_BYTE_send_blocking(USART1, get_CRC_data_value());
		UART_BYTE_send_blocking(USART1, '\r');
		Delay(250);
		clear_CRC_value();
		update_CRC_data_value(0x11111111);
		update_CRC_data_value(0x22222222);
		update_CRC_data_value(0xAAAAAAAA);
		update_CRC_data_value(0xBBBBBBBB);
		UART_BYTE_send_blocking(USART1, get_CRC_data_value());
		UART_BYTE_send_blocking(USART1, '\r');
		Delay(250);


#endif
#if(UART_DMA_TX_RX_INTERRUPT_TEST)
		Delay(500);
		//UART_BYTE_send_blocking(USART1, 'S');//I realize this is only a temporary solution
		UART_DMA_START();
		//UART_BYTE_send_blocking(USART1, 'S');//I realize this is only a temporary solution
		Delay(500);
		UART_DMA_RESET();
		//UART_BYTE_send_blocking(USART1, 'S');//I realize this is only a temporary solution
#endif
#if(ROM_OUTPUT_TESTER)
		set_cart_rom_pins_output();
		write_rom_address(0xABCDEF);
		//write_rom_address(0b101010101010101010101010);
		//write_rom_address(0b111111111111000000000000);
		//write_rom_address(0b111111111110000001111111);
		//write_rom_address  (0b000000000001111110000000);
		//write_rom_address  (0b000001111111111111111111);
		write_rom_address  (0b111111111111111111111111);


		//RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;//enalbe clock source
		//GPIOB->CRL = 0;
		//GPIOB->CRL |= (0b11<<GPIO_CRL_MODE3_Pos);//cnf1/cnf0. //push_pull
		//GPIOB->CRL |= (0b00<<GPIO_CRL_CNF3_Pos); //MODE0/MODE1.//50MHZ
		//GPIOB->ODR |= (1ul<<GPIO_ODR_ODR3_Pos);
		//GPIOB->ODR = 0b11111111111111111111111111111111;
		while(1);
#endif

#if(ROM_INPUT_TESTER)
		rom_output_tester = read_cart_rom();
		//while(1);
#endif

#if(TEST_CONTROLL_IO)
		CLK_HIGH();
		WR_LOW();
		RD_LOW();
		CS_LOW();
		while(1);
#endif

#if(PROOF_OF_CONCEPT)
/*		while(1)
		{
			set_cart_rom_pins_input();
			set_cart_rom_pins_output();
			set_cart_rom_pins_input();
		}
*/
		// Read rom
		//tempWord = 'AB';
		/*while(1)
		{
			UART_BYTE_send_blocking(USART1, tempWord);
			Delay(1000);
		}*/
		(void) UART_BYTE_recieve_blocking(USART1);
		for (uint32_t myAddress = 0; myAddress < cartSize; myAddress += 512)
		{

			for (int currWord = 0; currWord < 512; currWord += 2)
			{
				tempWord = read_rom_word(myAddress + currWord);
				//tempWord = 'AB';
				UART_BYTE_send_blocking(USART1, tempWord);
				tempWord = (tempWord>>8);
				UART_BYTE_send_blocking(USART1, tempWord);
		    }
		}
#endif
#if(PYTHON_TEST_ONE	)
		while(1)
		{
			UART_BYTE_send_blocking(USART1, 'A');
			//while(1);
			Delay(1200000);

		}
#endif
	}
}


uint16_t read_rom_word(uint32_t input)
{
	uint16_t return_value;
	set_cart_rom_pins_output();
	Delay(1);

	// Divide address by two to get word addressing
	input = input >> 1;

	write_rom_address(input);
	Delay(5);
	CS_LOW();
	Delay(5);
	set_cart_rom_pins_input();
	Delay(5);
	RD_LOW();
	Delay(5);
	return_value = read_cart_rom();
	CS_HIGH();
	RD_HIGH();
	Delay(5);

	return return_value;



}



