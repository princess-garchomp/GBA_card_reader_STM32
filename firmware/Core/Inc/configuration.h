#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//5_3_2025:
//the intent with the "configuration.h/.c" was to be able to test each driver I was working on
//issue I had was "how do I keep track of all the testing I do, and how do I save it"
//if I was to start this project again I would just do better version control stuff.
//
//I guess I am going to do some UGLY stuff now...

//intent here is that I can chose what driver I can test out in main/control the configuration in the assocaited .c file
#define GPIO_BASIC_TEST					0
#define HSI_SYSTEM_CLOCK_TEST			0
#define HSE_AS_SYSTEM_CLOCK				0
#define HSE_PLL_SYS_CLOCK				0
#define UART_HSE_PLL_BLOCKING			0
#define INTERRUPTING_RX_UART			0
#define INTERRUPTING_TX_UART			0
#define CRC_TEST_ONE					0
#define UART_DMA_TX_RX_INTERRUPT_TEST	0
#define ROM_OUTPUT_TESTER				0
#define ROM_INPUT_TESTER				0
#define TEST_CONTROLL_IO				0
#define PROOF_OF_CONCEPT				1
#define PYTHON_TEST_ONE					0

void UGLY_configuration_init();
void UGLY_coniguration_main_foo();


#endif
