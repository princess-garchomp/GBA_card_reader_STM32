#include "GPIO.h"

#define TWO_CONFIG_BITS(X)			X*2

#define PA12_CNF_OFFSET				18
#define PA12_MODE_OFFSET			16
#define PA12_ODR_OFFSET				12

#define PA2_CNF_OFFSET				10
#define PA2_MODE_OFFSET				8
#define PA2_ODR_IDR_OFFSET			2

//GPIOx_CRL and GPIOx_CRH
#define CNF_OUTPUT_PUSH_PULL		0b00
#define CNF_OUTPUT_OPEN_DRAIN		0b01
#define CNF_INPUT_FLOATING			0b01
#define CNF_INPUT_PULL_DOWN			0b10
#define CNF_INPUT_PULL_UP			0b10

#define MODE_NONE					0b00
#define MODE_SPEED_10MHZ			0b01
#define MODE_SPEED_2MHZ				0b10
#define MODE_SPEED_50MHZ			0b11

//GPIOx_ODR
#define ODR_INPUT_PULL_UP			0b1
#define ODR_INPUT_PULL_DOWN			0b0
#define ODR_OUTPUT_ON				0b1
#define ODR_OUTPUT_OFF				0b0

//GPIOx_IDR
#define IDR_INPUT_ON				0b1
#define IDR_INPUT_OFF				0b0




//configuration masks:
#define CRL_A_IO_MASK				(GPIO_CRL_MODE0_Msk|GPIO_CRL_CNF0_Msk|GPIO_CRL_MODE3_Msk|GPIO_CRL_CNF3_Msk|GPIO_CRL_MODE4_Msk|GPIO_CRL_CNF4_Msk|GPIO_CRL_MODE5_Msk|GPIO_CRL_CNF5_Msk|GPIO_CRL_MODE6_Msk|GPIO_CRL_CNF6_Msk|GPIO_CRL_MODE7_Msk|GPIO_CRL_CNF7_Msk)
#define CRH_A_IO_MASK				(GPIO_CRH_MODE8_Msk|GPIO_CRH_CNF8_Msk|GPIO_CRH_MODE11_Msk|GPIO_CRH_CNF11_Msk|GPIO_CRH_MODE12_Msk|GPIO_CRH_CNF12_Msk)
#define CRL_B_IO_MASK				(GPIO_CRL_MODE0_Msk|GPIO_CRL_CNF0_Msk|GPIO_CRL_MODE1_Msk|GPIO_CRL_CNF1_Msk|GPIO_CRL_MODE2_Msk|GPIO_CRL_CNF2_Msk|GPIO_CRL_MODE4_Msk|GPIO_CRL_CNF4_Msk|GPIO_CRL_MODE5_Msk|GPIO_CRL_CNF5_Msk|GPIO_CRL_MODE6_Msk|GPIO_CRL_CNF6_Msk|GPIO_CRL_MODE7_Msk|GPIO_CRL_CNF7_Msk)
#define CRH_B_IO_MASK				(GPIO_CRH_MODE8_Msk|GPIO_CRH_CNF8_Msk|GPIO_CRH_MODE9_Msk|GPIO_CRH_CNF9_Msk|GPIO_CRH_MODE10_Msk|GPIO_CRH_CNF10_Msk|GPIO_CRH_MODE11_Msk|GPIO_CRH_CNF11_Msk|GPIO_CRH_MODE12_Msk|GPIO_CRH_CNF12_Msk|GPIO_CRH_MODE13_Msk|GPIO_CRH_CNF13_Msk|GPIO_CRH_MODE14_Msk|GPIO_CRH_CNF14_Msk|GPIO_CRH_MODE15_Msk|GPIO_CRH_CNF15_Msk)
#define CRH_C_IO_MASK				(GPIO_CRH_MODE13_Msk|GPIO_CRH_CNF13_Msk)






/////////////////////////////////////////////////////////////////////////////////////////////////////
//write rom address masks:
#define CRX_PUSH_PULL_OUTPUT_50_MHZ	(CNF_OUTPUT_PUSH_PULL|MODE_SPEED_50MHZ)//this is 4 pits long

#define PA0_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE0_Pos)
#define PA3_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE3_Pos)
#define PA4_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE4_Pos)
#define PA5_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE5_Pos)
#define PA6_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE6_Pos)
#define PA7_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE7_Pos)

#define PORT_A_CRL_OUTPUT_CONFIG		(PA0_CRH_OUTPUT_CONFIG|PA3_CRL_OUTPUT_CONFIG|PA4_CRL_OUTPUT_CONFIG|PA5_CRL_OUTPUT_CONFIG|PA6_CRL_OUTPUT_CONFIG|PA7_CRL_OUTPUT_CONFIG)

#define PA8_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE8_Pos)
#define PA11_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE11_Pos)
#define PA12_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE12_Pos)


#define PORT_A_CRH_OUTPUT_CONFIG		(PA8_CRH_OUTPUT_CONFIG|PA11_CRH_OUTPUT_CONFIG|PA12_CRH_OUTPUT_CONFIG)



#define PB0_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE0_Pos)
#define PB1_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE1_Pos)
#define PB2_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE2_Pos)
#define PB4_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE4_Pos)
#define PB5_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE5_Pos)
#define PB6_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE6_Pos)
#define PB7_CRL_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE7_Pos)

#define PORT_B_CRL_OUTPUT_CONFIG		(PB0_CRL_OUTPUT_CONFIG|PB1_CRL_OUTPUT_CONFIG|PB2_CRL_OUTPUT_CONFIG|PB4_CRL_OUTPUT_CONFIG|PB5_CRL_OUTPUT_CONFIG|PB6_CRL_OUTPUT_CONFIG|PB7_CRL_OUTPUT_CONFIG)

#define PB10_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE10_Pos)
#define PB11_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE11_Pos)
#define PB12_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE12_Pos)
#define PB13_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE13_Pos)
#define PB14_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE14_Pos)
#define PB15_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE15_Pos)
#define PB8_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE8_Pos)
#define PB9_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE9_Pos)

#define PORT_B_CRH_OUTPUT_CONFIG		(PB10_CRH_OUTPUT_CONFIG|PB11_CRH_OUTPUT_CONFIG|PB12_CRH_OUTPUT_CONFIG|PB13_CRH_OUTPUT_CONFIG|PB14_CRH_OUTPUT_CONFIG|PB15_CRH_OUTPUT_CONFIG|PB8_CRH_OUTPUT_CONFIG|PB9_CRH_OUTPUT_CONFIG)


#define PC13_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE13_Pos)

#define PORT_C_CRH_OUTPUT_CONFIG	(PC13_CRH_OUTPUT_CONFIG)



#define PORT_A_ODR_OUTPUT_MASK		(GPIO_PA0_MASK|GPIO_PA3_MASK|GPIO_PA4_MASK|GPIO_PA5_MASK|GPIO_PA6_MASK|GPIO_PA7_MASK|GPIO_PA8_MASK|GPIO_PA11_MASK|GPIO_PA12_MASK)
#define PORT_B_ODR_OUTPUT_MASK 		(GPIO_PB0_MASK|GPIO_PB1_MASK|GPIO_PB10_MASK|GPIO_PB11_MASK|GPIO_PB12_MASK|GPIO_PB13_MASK|GPIO_PB14_MASK|GPIO_PB15_MASK|GPIO_PB2_MASK|GPIO_PB4_MASK|GPIO_PB5_MASK|GPIO_PB6_MASK|GPIO_PB7_MASK|GPIO_PB8_MASK|GPIO_PB9_MASK)
#define PORT_C_ODR_OUTPUT_MASK 		(GPIO_PC13_MASK)
/////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////
//read rom address masks:
//#define CRX_PULL_DOWN_INPUT	(CNF_INPUT_PULL_DOWN|MODE_NONE)//this is 4 pits long
//#define CRX_PULL_DOWN_INPUT	(CNF_INPUT_FLOATING|MODE_NONE)//this is 4 pits long
#define CRX_PULL_DOWN_INPUT	(0b1000)//this is 4 pits long



#define PA3_CRL_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRL_MODE3_Pos)
#define PA4_CRL_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRL_MODE4_Pos)
#define PA5_CRL_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRL_MODE5_Pos)
#define PA6_CRL_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRL_MODE6_Pos)
#define PA7_CRL_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRL_MODE7_Pos)

#define PORT_A_CRL_INPUT_CONFIG		(PA3_CRL_INPUT_CONFIG|PA4_CRL_INPUT_CONFIG|PA5_CRL_INPUT_CONFIG|PA6_CRL_INPUT_CONFIG|PA7_CRL_INPUT_CONFIG)

#define PA8_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE8_Pos)
#define PA11_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE11_Pos)
#define PA12_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE12_Pos)

#define PORT_A_CRH_INPUT_CONFIG		(PA8_CRH_INPUT_CONFIG|PA11_CRH_INPUT_CONFIG|PA12_CRH_INPUT_CONFIG)



#define PB0_CRL_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRL_MODE0_Pos)
#define PB1_CRL_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRL_MODE1_Pos)

#define PORT_B_CRL_INPUT_CONFIG		(PB0_CRL_INPUT_CONFIG|PB1_CRL_INPUT_CONFIG)

#define PB10_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE10_Pos)
#define PB11_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE11_Pos)
#define PB12_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE12_Pos)
#define PB13_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE13_Pos)
#define PB14_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE14_Pos)
#define PB15_CRH_INPUT_CONFIG		(CRX_PULL_DOWN_INPUT<<GPIO_CRH_MODE15_Pos)

#define PORT_B_CRH_INPUT_CONFIG		(PB10_CRH_INPUT_CONFIG|PB11_CRH_INPUT_CONFIG|PB12_CRH_INPUT_CONFIG|PB13_CRH_INPUT_CONFIG|PB14_CRH_INPUT_CONFIG|PB15_CRH_INPUT_CONFIG)


#define PORT_A_ODR_INPUT_MASK		(GPIO_PA3_MASK|GPIO_PA4_MASK|GPIO_PA5_MASK|GPIO_PA6_MASK|GPIO_PA7_MASK|GPIO_PA8_MASK|GPIO_PA11_MASK|GPIO_PA12_MASK)
#define PORT_B_ODR_INPUT_MASK		(GPIO_PB0_MASK|GPIO_PB1_MASK|GPIO_PB10_MASK|GPIO_PB11_MASK|GPIO_PB12_MASK|GPIO_PB13_MASK|GPIO_PB14_MASK|GPIO_PB15_MASK)
/////////////////////////////////////////////////////////////////////////////////////////////////////







//ODR masks:
#define	GPIO_PA0_MASK		GPIO_ODR_ODR0_Msk
#define	GPIO_PA3_MASK		GPIO_ODR_ODR3_Msk
#define	GPIO_PA4_MASK		GPIO_ODR_ODR4_Msk
#define	GPIO_PA5_MASK		GPIO_ODR_ODR5_Msk
#define	GPIO_PA6_MASK		GPIO_ODR_ODR6_Msk
#define	GPIO_PA7_MASK		GPIO_ODR_ODR7_Msk
#define	GPIO_PA8_MASK		GPIO_ODR_ODR8_Msk
#define	GPIO_PA11_MASK		GPIO_ODR_ODR11_Msk
#define	GPIO_PA12_MASK		GPIO_ODR_ODR12_Msk

#define	GPIO_PB0_MASK		GPIO_ODR_ODR0_Msk
#define	GPIO_PB1_MASK		GPIO_ODR_ODR1_Msk
#define	GPIO_PB10_MASK		GPIO_ODR_ODR10_Msk
#define	GPIO_PB11_MASK		GPIO_ODR_ODR11_Msk
#define	GPIO_PB12_MASK		GPIO_ODR_ODR12_Msk
#define	GPIO_PB13_MASK		GPIO_ODR_ODR13_Msk
#define	GPIO_PB14_MASK		GPIO_ODR_ODR14_Msk
#define	GPIO_PB15_MASK		GPIO_ODR_ODR15_Msk
#define	GPIO_PB2_MASK		GPIO_ODR_ODR2_Msk
#define	GPIO_PB4_MASK		GPIO_ODR_ODR4_Msk
#define	GPIO_PB5_MASK		GPIO_ODR_ODR5_Msk
#define	GPIO_PB6_MASK		GPIO_ODR_ODR6_Msk
#define	GPIO_PB7_MASK		GPIO_ODR_ODR7_Msk
#define	GPIO_PB8_MASK		GPIO_ODR_ODR8_Msk
#define	GPIO_PB9_MASK		GPIO_ODR_ODR9_Msk

#define	GPIO_PC13_MASK		GPIO_ODR_ODR13_Msk


//all following 24 pins are used to write an address to the rom
//only pins A0 to A15 (16 pins) are data output by the rom after writting an address to it
//A0		PA3				CRL
//A1		PA4				CRL
//A2		PA5				CRL
//A3		PA6				CRL
//A4		PA7				CRL
//A5		PB0				CRL
//A6		PB1				CRL
//A7		PB10			CRH
//A8		PB11			CRH
//A9		PB12			CRH
//A10		PB13			CRH
//A11		PB14			CRH
//A12		PB15			CRH
//A13		PA8				CRH
//A14		PA11			CRH
//A15		PA12			CRH
//D0		PA15			CRH////////////////////replaced wiht A0. also used for the jtag interface....
//D1		PB3				CRL////////////////////replaced with PB2. PB3 is used for the jtag interface
//D2		PB4				CRL////////////////////replaced with PC13. PB4 is also used for the jtag interface...
//D3		PB5				CRL
//D4		PB6				CRL
//D5		PB7				CRL
//D6		PB8				CRH
//D7		PB9				CRH



////////////////////////////////////////////////////////////////////////
//masks/defines for the contorl pins that I have left

//GBA_CLK PC14
//GBA_CS  PC15
//GBA_WR  PA1
//GNA_RD  PA2

//this is the config we are using to output to the cart
//#define CRX_PUSH_PULL_OUTPUT_50_MHZ	(CNF_OUTPUT_PUSH_PULL|MODE_SPEED_50MHZ)//this is 4 pits long

//masks for control pins
#define CRL_A_CONTROL_PINS_IO_MASK			(GPIO_CRL_MODE1_Msk|GPIO_CRL_CNF1_Msk|GPIO_CRL_MODE2_Msk|GPIO_CRL_CNF2_Msk)
#define CRH_C_CONTROL_PINS_IO_MASK			(GPIO_CRH_MODE14_Msk|GPIO_CRH_CNF14_Msk|GPIO_CRH_MODE15_Msk|GPIO_CRH_CNF15_Msk)

#define PA1_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE1_Pos)
#define PA2_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRL_MODE2_Pos)


#define PC14_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE14_Pos)
#define PC15_CRH_OUTPUT_CONFIG		(CRX_PUSH_PULL_OUTPUT_50_MHZ<<GPIO_CRH_MODE15_Pos)


#define PORT_A_CONTROL_PINS_CRL_OUTPUT_CONFIG		(PA1_CRH_OUTPUT_CONFIG|PA2_CRH_OUTPUT_CONFIG)
#define	PORT_C_CONTROL_PINS_CRL_CRL_OUTPUT_CONFIG	(PC14_CRH_OUTPUT_CONFIG|PC15_CRH_OUTPUT_CONFIG)

//ODR masks for the control pins
#define	GPIO_PA1_MASK		GPIO_ODR_ODR1_Msk
#define	GPIO_PA2_MASK		GPIO_ODR_ODR2_Msk

#define	GPIO_PC14_MASK		GPIO_ODR_ODR14_Msk
#define	GPIO_PC15_MASK		GPIO_ODR_ODR15_Msk


////////////////////////////////////////////////////////////////////////

void gpio_pin_one_output_config(void)
{
	//intent: led cathoed in to pa12 and led annoed into gnd
	//micro pin33(pa12), jumper pin 19 (A15)
	//enable the gpioA clokc
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//enalbe clock source

	//pA12 is in the CRH, not the CRL register
	//CHR resets at value 0x44444444, if we are ORing values into it we need to do a reset.
	GPIOA->CRH = 0;
	GPIOA->CRH |= (CNF_OUTPUT_PUSH_PULL<<PA12_CNF_OFFSET);//cnf1/cnf0. //push_pull
	GPIOA->CRH |= (MODE_SPEED_50MHZ<<PA12_MODE_OFFSET); //MODE0/MODE1.//50MHZ
	reset_gpio_pin_one();
}


void gpio_pin_two_input_config(void)
{
	//intent is to connect a push button between this pin and gnd. on button push the IDR will be a vale of 1
	//micro pin 12 (pa2), jumper pin 33 (GBA_RD)
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//enable clock source

	//pa2 is the CRL register not the CRH register
	//CHL resets at value 0x44444444, if we are ORing values into it we need to do a reset.
	GPIOA->CRL = 0;
	GPIOA->CRL |= (CNF_INPUT_PULL_UP<<PA2_CNF_OFFSET);//cnf1/cnf0. //push_pull
	GPIOA->CRL |= (0b00<<PA12_MODE_OFFSET); //no value
	GPIOA->ODR |= (ODR_INPUT_PULL_UP<<PA2_ODR_IDR_OFFSET);//IDR set to 1 when connected to gnd
	//GPIOA->ODR |= (ODR_INPUT_PULL_DOWN<<PA2_ODR_IDR_OFFSET);//IDR set to 0 when connected to gnd

}

void set_gpio_pin_one(void)
{
	GPIOA->ODR |= (1ul<<PA12_ODR_OFFSET);
}
void reset_gpio_pin_one(void)
{
	GPIOA->ODR &= ~(1ul<<PA12_ODR_OFFSET);
}

_Bool check_gpio_two_input(void)
{
	//return the value in the IDR register
	//value of 1 means that the pin is connected to gnd
	return (GPIOA->IDR & (1ul<<PA2_ODR_IDR_OFFSET));
}

void init_IO()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//enalbe clock source
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;//enalbe clock source
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;//enalbe clock source

	//then I cant debug...
	//also I may have bricked a board. I cannot program it at the moment.
	//we need to disable the JTAG interface so pin PB3 will work.
	 /*RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR &= ~(AFIO_MAPR_SWJ_CFG_Msk);
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_2;*/
	set_cart_rom_pins_output();

	init_control_pins();


}

void set_cart_rom_pins_input()
{
	//clear port A CRL bits for PA3 to PA7.
	GPIOA->CRL &= ~(CRL_A_IO_MASK);
	//set port A CRL as input with a pull down resistor to pins PA3 to PA7
	GPIOA->CRL |= (PORT_A_CRL_INPUT_CONFIG);
	//clear port A CRH bits for PA8, PA11, PA12
	GPIOA->CRH &= ~(CRH_A_IO_MASK);
	//set port A CRH as input with a pull down resistor to pins PA11,PA12
	GPIOA->CRH |= (PORT_A_CRH_INPUT_CONFIG);
	//set PINA ODR so that we have pull down resistors
	GPIOA->ODR & ~(PORT_A_ODR_INPUT_MASK);

	GPIOA->IDR = 0;

	//clear port B CRL bits for PB0, PB1, PB3 to PB7
	GPIOB->CRL &= ~(CRL_B_IO_MASK);
	//set port B CRL as input with a pull down resistor to pins PB0, PB1, PB3 to PB7
	GPIOB->CRL |= (PORT_B_CRL_INPUT_CONFIG);
	//clear port B CRH bits for PB8, PB9, PB10 to PB15
	GPIOB->CRH &= ~(CRH_B_IO_MASK);
	//set port B CRH as input with a pull down resistor to pins PB8, PB9, PB10 to PB15
	GPIOB->CRH |= (PORT_B_CRH_INPUT_CONFIG);
	//set PINB ODR so that we have pull down resistors
	GPIOB->ODR & ~(PORT_B_ODR_INPUT_MASK);

	GPIOB->IDR = 0;

}
void set_cart_rom_pins_output()
{
	//clear port A CRL bits for PA3 to PA7.
	GPIOA->CRL &= ~(CRL_A_IO_MASK);
	//set port A CRL as push pull output and 50MHZ to pins PA0 and PA3 to PA7
	GPIOA->CRL |= (PORT_A_CRL_OUTPUT_CONFIG);
	//clear port A CRH bits for PA8, PA11, PA12 and PA15
	GPIOA->CRH &= ~(CRH_A_IO_MASK);
	//set port A CRH as push pull output and 50MHZ to pins PA11,PA12
	GPIOA->CRH |= (PORT_A_CRH_OUTPUT_CONFIG);

	//clear port B CRL bits for PB0, PB1, PB2 adn PB4 to PB7
	GPIOB->CRL &= ~(CRL_B_IO_MASK);
	//set port B CRL as push pull output and 50MHZ to pins PB0, PB1, PB2 and PB4 to PB7
	GPIOB->CRL |= (PORT_B_CRL_OUTPUT_CONFIG);
	//clear port B CRH bits for PB8, PB9, PB10 to PB15
	GPIOB->CRH &= ~(CRH_B_IO_MASK);
	//set port B CRH as push pull output and 50MHZ to pins PB8, PB9, PB10 to PB15
	GPIOB->CRH |= (PORT_B_CRH_OUTPUT_CONFIG);

	//clear PORTC CRH bits for PC13
	GPIOC->CRH &= ~(CRH_C_IO_MASK);
	//seet CRH port C as push pull output and 50MHZ foor PC13
	GPIOC->CRH |= (PORT_C_CRH_OUTPUT_CONFIG);

	//clear ODR port A pins
	//clear PA3 to PA7, clear PA8, clear PA11 and PA12, clear PA0
	GPIOA->ODR &= ~(PORT_A_ODR_OUTPUT_MASK);

	//clear ODR port B pins
	//c;ear PB0 and PB1, clear PB10 to PB15, clear PB2 adn PB4 to PB9
	GPIOB->ODR &= ~(PORT_B_ODR_OUTPUT_MASK);

	//clear ODR port C pins, PC13
	GPIOC->ODR &= ~(PORT_C_ODR_OUTPUT_MASK);

}

void write_rom_address(uint32_t input)
{
	//on the next rev of my board this is going to be way simpler.
	//I did the schematic such that it made the board layout phase easier at the cost of firmware complexity.
	//I did not realize how sloppy this would make my firmware.

	set_cart_rom_pins_output();

	uint32_t temp = 0;



	//set A0 to A4 on the cart
	//set pins PA3 to PA7 on the micro
	temp = (input & ~(0b11111111111111111111111111100000));//save bits in position form 0 to 4 from the input
	GPIOA->ODR |= (temp <<3);//left shift 3. get it to ODR positions for PA3 to PA7

	//set A5 to A6 on the cart
	//set pins PB0 to PB1 on the micro
	temp = (input & ~(0b11111111111111111111111110011111));//save bits in position form 5 and 6 from the input
	GPIOB->ODR |= (temp >>5);//right shift 5. get it to ODR positions for PB0 and PB1

	//set A7 to A12 on the cart
	//set pins PB10 to PB15 on the micro
	temp = (input & ~(0b11111111111111111110000001111111));//save bits in position form 7 and 12 from the input
	GPIOB->ODR |= (temp <<3);//left shift 4. get it to ODR positions for PB10 to PB15

	//set A13 on the cart
	//set pins PA8 on the micro
	temp = (input & ~(0b11111111111111111101111111111111));// save bit in position 13
	GPIOA->ODR |= (temp >>5);//right shift 5. get it to ODR position PA8

	//set A14 to A15 on the cart
	//set pins PA11 and PA12 on the micro
	temp = (input & ~(0b11111111111111110011111111111111));//save bits in positions 14 and 15
	GPIOA->ODR |= (temp >>3);//right shift 3. get it to ODR positions for PA11 and PA12

	//set D0 on the cart
	//set pins PA0 on the micro
	temp = (input & ~(0b11111111111111101111111111111111));//save bits in positions 16
	GPIOA->ODR |= (temp >>16);//right shit 1. get it to ODR position for PA0

	//D1 on the cart
	//set PB2 on the micro
	temp = (input & ~(0b11111111111111011111111111111111));//save bits in positions 17
	GPIOB->ODR |= (temp >>15);//right shift 15. get it to ODR positions PB2

	//set D2 cart
	//set pins PC13 on the micro
	temp = (input & ~(0b11111111111110111111111111111111));//save bits in positions 18
	GPIOC->ODR |= (temp >>5);//right shift 5. get it to ODR positions PC13

	//set D3 to D7 on the cart
	//set pins PB5 to PB9 on the micro
	temp = (input & ~(0b11111111000001111111111111111111));//save bits in positions 19 to 23
	GPIOB->ODR |= (temp >>14);//right shift 14. get it to ODR positions PB5 to PB9

}
uint16_t read_cart_rom(void)
{
	//on the next rev of my board this is going to be way simpler.
	//I did the schematic such that it made the board layout phase easier at the cost of firmware complexity.
	//I did not realize how sloppy this would make my firmware.

	//				//(A15 |A14 |A13|A12 |A11 |A10 |A9  |A8  |A7  |A6 |A5 |A4 |A3 |A2 |A1 |A0)	//on the cart
	//return_value |= (PA12|PA11|PA8|PB15|PB14|PB13|PB12|PB11|PB10|PB0|PB1|PA7|PA6|PA5|PA4|PA3)	//on the micro
	uint16_t return_value = 0;
	uint16_t temp = 0;

	set_cart_rom_pins_input();

	temp = (GPIOA->IDR & ~(0b11111111111111111111111100000111));//save bits on pins PA3 to PA7
	return_value |= (temp>>3);//right shift by 3. put them in position A0 to A4

	temp = (GPIOB->IDR & ~(0b11111111111111111111111111111100));//save bits on pins PB0 and PB1
	return_value |= (temp<<5);//left shift by 5. put them in position A5 and A6

	temp = (GPIOB->IDR & ~(0b11111111111111110000001111111111));//save bits on pins PB10 ti PB15
	return_value |= (temp>>3);//right shift by 3. put them in positions A7 to A12

	temp = (GPIOA->IDR & ~(0b11111111111111111111111011111111));//save bits on pins PA8
	return_value |= (temp<<5);//left  shift by 5. put them in position A13

	temp = (GPIOA->IDR & ~(0b11111111111111111110011111111111));//save bits on pins PA11 and PA12
	return_value |= (temp<<3);//right shift by 3. put them in position A14 and A15

	return return_value;
}

void init_control_pins()
{

 	//clear port A CRL bits for PA1 and PA2
 	GPIOA->CRL &= ~(CRL_A_CONTROL_PINS_IO_MASK);
 	//set port A CRL as push pull output and 50MHZ to pins PA1 and PA2
 	GPIOA->CRL |= (PORT_A_CONTROL_PINS_CRL_OUTPUT_CONFIG);

 	//clear port A CRL bits for PC14 adn PC15
 	GPIOC->CRH &= ~(CRH_C_CONTROL_PINS_IO_MASK);
 	//set port C CRL as push pull output and 50MHZ to pins PC14 and PC15
 	GPIOC->CRH |= (PORT_C_CONTROL_PINS_CRL_CRL_OUTPUT_CONFIG);

 	CLK_LOW();
 	WR_HIGH();
 	RD_HIGH();
 	CS_HIGH();

}
void CLK_HIGH()
{
	GPIOC->ODR |= GPIO_PC14_MASK;
}

void CLK_LOW()
{
	GPIOC->ODR &= ~GPIO_PC14_MASK;

}

void CS_HIGH()
{
	GPIOC->ODR |= GPIO_PC15_MASK
;
}
void CS_LOW()
{
	GPIOC->ODR &= ~GPIO_PC15_MASK
;
}

void WR_HIGH()
{
	GPIOA->ODR |= GPIO_PA1_MASK;
}
void WR_LOW()
{
	GPIOA->ODR &= ~GPIO_PA1_MASK;
}

void RD_HIGH()
{
	GPIOA->ODR |= GPIO_PA2_MASK;
}
void RD_LOW()
{
	GPIOA->ODR &= ~GPIO_PA2_MASK;
}

