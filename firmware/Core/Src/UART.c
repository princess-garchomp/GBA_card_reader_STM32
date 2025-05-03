#include "UART.h"

#define CONCAT(A,B) A ## B



#define GPIO_PORT(PORT_LETTER)				CONCAT(GPIO,PORT_LETTER)
#define CNF_ALT_FUNCTOIN_PUSH_PULL			0b10
#define CNF_ALT_FUNCTION_OPEN_DRAIN			0b11

#define MODE_SPEED_10MHZ			0b01
#define MODE_SPEED_2MHZ				0b10
#define MODE_SPEED_50MHZ			0b11

#define CNF_INPUT_FLOATING			0b01
#define CNF_INPUT_PULL_DOWN			0b10
#define CNF_INPUT_PULL_UP			0b10

//PA9
#define USART1_TX 					9
#define USART1_TX_PORT				A
#define PA9_CNF_OFFSET				6
#define PA9_MODE_OFFSET				4


//PA10
#define USART1_RX 					10
#define USART1_RX_PORT 				A
#define PA10_CNF_OFFSET				10
#define PA10_MODE_OFFSET			8

//some of these confugurations use two bits to control the function
#define TWO_CONFIG_BITS(X) X*2

volatile uint8_t last_USART_RX = 0x00;
volatile uint8_t USART_TX = 0x00;
//volatile uint16_t usart_dma_array[5] = {'AA','BB','CC','DD','EE'};
char string[] = "vroomvroom";


//BAUD=clcok/9600
//16mhz/9600=1667
void generic_USART_config(USART_TypeDef * USARTx, uint32_t BAUD_SET)
{
    //disable USART
    USARTx->CR1 &= ~USART_CR1_UE;

    //maybe it needs to be done here?
    USART1->CR3 |= USART_CR3_DMAT;//DMA mode enabled for transmit

    //set data length to 8 bits
    //00 = 8bits, 01 = 9bits, 10 = 7 bits
    USARTx->CR1 &= ~USART_CR1_M;

    //select 1 stop bit
    //00 = 1 stop bit, 01 .5 stop bits, 10 2 stop bits, 11 = 1.5 stop bits
    USARTx->CR2 &= ~USART_CR2_STOP;

    //set parity as no parity
    //0 = no parity, 1= parity
    USARTx->CR1 &= ~USART_CR1_PCE;

    //oversampling by 16
    //0= oversampling by 16, 1= oversampling by 8
    //USARTx->CR1 &= ~USART_CR1_OVER8;

    //set baud rate
    USARTx->BRR = BAUD_SET;

    //enable send and recieve
    USARTx->CR1 |= (USART_CR1_TE|USART_CR1_RE);

    //enable usart
    USARTx->CR1 |= USART_CR1_UE;
}
void config_UART1_blocking_9600baud()
{
	//config PA9 as uartTX, set alternatice function to 0x10 to be push pull.
	//set PA10 to be input connected to open drain
    //config PA9 USRT1_TX (AF7), config PA10 USART_RX (AF7)
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//enalbe clock source
    GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_CNF_OFFSET); //clear bits in the CNF location fo PA9
    GPIO_PORT(USART1_TX_PORT)->CRH |= (CNF_ALT_FUNCTOIN_PUSH_PULL<<PA9_CNF_OFFSET);//select alt function
    GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_MODE_OFFSET); //clear bits in the MODE location for PA9
    GPIO_PORT(USART1_TX_PORT)->CRH |= (MODE_SPEED_50MHZ<<PA9_MODE_OFFSET);//set speed to PA9
    GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_CNF_OFFSET);//clear CNF for PA10
    GPIO_PORT(USART1_RX_PORT)->CRH |= (CNF_INPUT_PULL_UP<<PA10_CNF_OFFSET);//set PA10 as input pull up
    GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_MODE_OFFSET);//make sure PA10 MODE is set to zero


    //config clocks for USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    //config uart
    //72MHZ/9600 = 7500.
    generic_USART_config(USART1, 7500);
}
void config_UART1_blocking_115200baud()
{
	//config PA9 as uartTX, set alternatice function to 0x10 to be push pull.
	//set PA10 to be input connected to open drain
    //config PA9 USRT1_TX (AF7), config PA10 USART_RX (AF7)
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//enalbe clock source
    GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_CNF_OFFSET); //clear bits in the CNF location fo PA9
    GPIO_PORT(USART1_TX_PORT)->CRH |= (CNF_ALT_FUNCTOIN_PUSH_PULL<<PA9_CNF_OFFSET);//select alt function
    GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_MODE_OFFSET); //clear bits in the MODE location for PA9
    GPIO_PORT(USART1_TX_PORT)->CRH |= (MODE_SPEED_50MHZ<<PA9_MODE_OFFSET);//set speed to PA9
    GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_CNF_OFFSET);//clear CNF for PA10
    GPIO_PORT(USART1_RX_PORT)->CRH |= (CNF_INPUT_PULL_UP<<PA10_CNF_OFFSET);//set PA10 as input pull up
    GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_MODE_OFFSET);//make sure PA10 MODE is set to zero


    //config clocks for USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    //config uart
    //72MHZ/115200 = 7500.
    generic_USART_config(USART1, 625);
}


void UART_BYTE_send_blocking(USART_TypeDef *USARTx, uint8_t input_byte)
{
    while(!(USARTx->SR & USART_SR_TC));//wait for hardware to set TXE,transmission is enabled
    USARTx->DR = input_byte;
    while(!(USARTx->SR & USART_SR_TC));//wait for transmission complete bit to be set, transimission has completed
    //USARTx->SR |= USART_ICR_TCCF;//set the transmission complete flah
}

//passing this a value of 7500 gives a baud of 9600
uint8_t UART_BYTE_recieve_blocking(USART_TypeDef *USARTx)
{
	uint8_t	retrun_value;
    //wait until we recieve a message.
    while(!(USARTx->SR & USART_SR_RXNE));
    retrun_value =USARTx->DR;
    USARTx->SR &= ~(USART_SR_RXNE);
    return retrun_value;


}

void config_UART1_interrupt(uint32_t BAUD_SET)
{
	//config PA9 as uartTX, set alternatice function to 0x10 to be push pull.
	//set PA10 to be input connected to open drain
	//config PA9 USRT1_TX (AF7), config PA10 USART_RX (AF7)
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//enalbe clock source
	GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_CNF_OFFSET); //clear bits in the CNF location fo PA9
	GPIO_PORT(USART1_TX_PORT)->CRH |= (CNF_ALT_FUNCTOIN_PUSH_PULL<<PA9_CNF_OFFSET);//select alt function
	GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_MODE_OFFSET); //clear bits in the MODE location for PA9
	GPIO_PORT(USART1_TX_PORT)->CRH |= (MODE_SPEED_50MHZ<<PA9_MODE_OFFSET);//set speed to PA9
	GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_CNF_OFFSET);//clear CNF for PA10
	GPIO_PORT(USART1_RX_PORT)->CRH |= (CNF_INPUT_PULL_UP<<PA10_CNF_OFFSET);//set PA10 as input pull up
	GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_MODE_OFFSET);//make sure PA10 MODE is set to zero

	//config clocks for USART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	USART1->CR1 |= USART_CR1_RXNEIE;//enable interrupt on recieve register not emty
	USART1->CR1 &= ~USART_CR1_TXEIE;//disable interrupt on send register empty
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);

	//config uart
	//72MHZ/9600 = 7500.
	generic_USART_config(USART1, BAUD_SET);
}

void UART_send_byte_interrupt_start(USART_TypeDef *USARTx, uint8_t input_byte)
{
    USARTx->CR1 |= USART_CR1_TXEIE; //enable transmit recieved empty interrupt
    USARTx->DR = input_byte;
}
void UART_interrupt_byte_send(USART_TypeDef *USARTx)
{
     if(USARTx->SR & USART_SR_TXE)
     {
        USARTx->CR1 &= ~USART_CR1_TXEIE;//disable interrupt on send register empty
     }
}
void set_USART_TX(uint8_t input)
{
	USART_TX = input;
}
uint8_t UART_interrupt_byte_recieve(USART_TypeDef *USARTx)
{
    if (USARTx->SR & USART_SR_RXNE)
    {
        return USARTx->DR;
    }
    return 0;

}
uint8_t get_last_USART_RX()
{
	return last_USART_RX;
}
void UART_DMA_TX_RX_INTERRUPT()
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;//enable the DMA1 channel clock
	USART1->CR3 |= USART_CR3_DMAT;//DMA mode enabled for transmit
	//USART1->CR3 |= USART_CR3_DMAR;// this would enable the DMA mode for the UART RX

	//DMA1_Channel7->CCR |= DMA_CCR_MSIZE_0;//memory size of 16bits
	DMA1_Channel4->CCR |= DMA_CCR_MINC;//memory incrament mode enabled
	DMA1_Channel4->CCR |= DMA_CCR_DIR;//set DMA to read from memory

	//DMA1_Channel4->CCR |= DMA_CCR_CIRC;//this makse it a circular
	//DMA1_Channel4->CCR |= DMA_CCR_PL;


	DMA1_Channel4->CNDTR = 5;//set the number of elements to send

	DMA1_Channel4->CPAR = (uint32_t)&USART1->DR;//set the address to send to
	//DMA1_Channel7->CMAR =(uint32_t)usart_dma_array;//set address that DMA reads from
	DMA1_Channel4->CMAR =(uint32_t)string;//set address that DMA reads from


	//config PA9 as uartTX, set alternatice function to 0x10 to be push pull.
	//set PA10 to be input connected to open drain
	//config PA9 USRT1_TX (AF7), config PA10 USART_RX (AF7)
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//enalbe clock source
	GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_CNF_OFFSET); //clear bits in the CNF location fo PA9
	GPIO_PORT(USART1_TX_PORT)->CRH |= (CNF_ALT_FUNCTOIN_PUSH_PULL<<PA9_CNF_OFFSET);//select alt function
	GPIO_PORT(USART1_TX_PORT)->CRH &= ~(3ul<<PA9_MODE_OFFSET); //clear bits in the MODE location for PA9
	GPIO_PORT(USART1_TX_PORT)->CRH |= (MODE_SPEED_50MHZ<<PA9_MODE_OFFSET);//set speed to PA9
	GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_CNF_OFFSET);//clear CNF for PA10
	GPIO_PORT(USART1_RX_PORT)->CRH |= (CNF_INPUT_PULL_UP<<PA10_CNF_OFFSET);//set PA10 as input pull up
	GPIO_PORT(USART1_RX_PORT)->CRH &= ~(3ul<<PA10_MODE_OFFSET);//make sure PA10 MODE is set to zero


	//config clocks for USART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	generic_USART_config(USART1, 7500);//just hard coding with 9600 for the moment
}
void UART_DMA_START()
{
	//DMA1_Channel7->CCR |= DMA_CCR_EN;
	DMA1_Channel4->CCR |= DMA_CCR_EN;
}
void UART_DMA_RESET()
{
	DMA1->IFCR |= DMA_IFCR_CTCIF4;//CTCIFx
	//DMA1->IFCR = 0;
	//DMA1->IFCR &= ~DMA_CCR_EN;
	//DMA1->IFCR |= DMA_IFCR_CGIF4;
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	DMA1_Channel4->CNDTR = 5;//set the number of elements to send

}
void USART1_IRQHandler()
{
#if(INTERRUPTING_RX_UART|INTERRUPTING_TX_UART)
    if(USART1->SR & USART_SR_TXE)
    {

        //USART1->DR = USART_TX;//the goal here is to show that the interrupt is firing
        UART_interrupt_byte_send(USART1);
        UART_BYTE_send_blocking(USART1, USART_TX);

    }
    if (USART1->SR & USART_SR_RXNE)
    {
        last_USART_RX = UART_interrupt_byte_recieve(USART1);

    }
#endif
#if(UART_DMA_TX_RX_INTERRUPT_TEST)
#endif
}
