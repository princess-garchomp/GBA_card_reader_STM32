#include "CLOCK.h"

volatile uint32_t TimeDelay;

void enable_HSI_as_sysclock_src()
{
	RCC->CR |= RCC_CR_HSION_Msk;

	// wait until HSI is ready
	while ( (RCC->CR & RCC_CR_HSIRDY_Msk) == 0 );

	// Select HSI as system clock source
	RCC->CFGR &= ~(0b11<<RCC_CFGR_SW_Pos);
	RCC->CFGR |=(RCC_CFGR_SW_HSI << RCC_CFGR_SW_Pos); // 01: HSI16 oscillator used as system clock

	// Wait till HSI is used as system clock source
	while ((RCC->CFGR & (0b11 << RCC_CFGR_SWS_Pos)) != (RCC_CFGR_SW_HSI<<RCC_CFGR_SWS_Pos));

}

void enable_HSE_as_sysclock_src(){

	//enable clokc source to HSE lines
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;

	// Enable HSE
	RCC->CR |= RCC_CR_HSEON_Msk;

	// wait until MSE is ready
	while ( (RCC->CR & RCC_CR_HSERDY_Msk) == 0 );

	// Select MSE as system clock source
	RCC->CFGR &= ~(0b11<<RCC_CFGR_SW_Pos);
	RCC->CFGR |=(RCC_CFGR_SW_HSE << RCC_CFGR_SW_Pos); // 01: HSI16 oscillator used as system clock

	// Wait till HSE is used as system clock source
	while ((RCC->CFGR & (0b11 << RCC_CFGR_SWS_Pos)) != (RCC_CFGR_SW_HSE<<RCC_CFGR_SWS_Pos));

}

void enable_PLL_sysclock_src_external_HSE_PLL_src()
{
	//HSE is a speed of 8MHZ
	//goal is a system clock speed of 72MHZ
	//72MHZ/8MHZ = 9
	//PLL multiplyer is a value of 9 to achieve the systick clock source of 72MHZ

	//the following bears an explanation. the short is at system clock of 72MHZ we are accessing flash to quickly.
	/*from the data sheet
	The FLASH_ACR register is used to enable/disable prefetch and half cycle access,
	and to control the Flash memory access time according to the CPU frequency

	These bits represent the ratio of the SYSCLK (system clock) period to the Flash access
	time:
	000 Zero wait state, if 0 < SYSCLK< 24 MHz
	001 One wait state, if 24 MHz < SYSCLK < 48 MHz
	010 Two wait states, if 48 MHz < SYSCLK < 72 MHz

	*/
	FLASH->ACR	|= FLASH_ACR_LATENCY_2;


	//apb1 prescaler.
	//we have the systemclock running at 72MHZ. the apb1 can only go at 36MHZ. need to divide it by 2.
	//if we do not have this line using the 72MHZ system clock causes problems.
	//the debugget crashes if we do not divide this by 2
	RCC->CFGR |= (0b100<<RCC_CFGR_PPRE1_Pos);

	//enable clokc source to HSE lines
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;

	// Enable HSE
	RCC->CR |= RCC_CR_HSEON_Msk;

	// wait until HSE ready
	while ( (RCC->CR & RCC_CR_HSERDY_Msk) == 0 );

	//set PLL multiplyer
	//RCC->CFGR |= RCC_CFGR_PLLMULL9_Msk;
	RCC->CFGR |= RCC_CFGR_PLLMULL9;
	//do not divide HSE clock when input to PLL
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;
	//set HSE pas PLL src
	RCC->CFGR |= RCC_CFGR_PLLSRC;

	//Enable PLL
	RCC->CR |= RCC_CR_PLLON;

	//wait until PLL ready
	while ( (RCC->CR & RCC_CR_PLLRDY_Msk) == 0 );

	// Select PLL as system clock source
	RCC->CFGR &= ~(RCC_CFGR_SW_Msk);//c;ear the SW bits in the CFGR reg
	RCC->CFGR |=(RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos); // set PLL as sorce for system clock

	//Wait till PLL is used as system clock source
	while ((RCC->CFGR & (0b11 << RCC_CFGR_SWS_Pos)) != (0b10<<RCC_CFGR_SWS_Pos));

}

void config_systic (uint32_t ticks)
{
  //disable systic IRQ and systic conter
  SysTick->CTRL =0;

  //set reload register
  SysTick->LOAD = ticks -1;

  //set interrupt priority of systic
  NVIC_SetPriority (SysTick_IRQn, (1ul<<__NVIC_PRIO_BITS)-1);

  ////reset systic counter
  SysTick->VAL=0;

  //select processor clock
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

  //enable exception reqiest
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

  ////enable systick timer
  //SysTick->CTRL |= (1ul<<0);
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
void config_systic_for_ms (uint32_t sysclock_freq_input)
{
  //disable systic IRQ and systic conter
  SysTick->CTRL =0;

  //set reload register
  /*
  systicinterruptperiod = (1+systick_LOAD)/systick_counter_clock_freq
  systick_LOAD = (systicinterruptperiod * systick_counter_clock_freq) -1

  systicinterruptperiod = 1ms =.001s
  systick_counter_clock_freq = sysclock_freq_input
  */
  SysTick->LOAD = ((.001*sysclock_freq_input)-1);

  //set interrupt priority of systic
  NVIC_SetPriority (SysTick_IRQn, (1ul<<__NVIC_PRIO_BITS)-1);

  ////reset systic counter
  SysTick->VAL=0;

  //select processor clock
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

  //enable exception reqiest
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

  ////enable systick timer
  //SysTick->CTRL |= (1ul<<0);
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/*Delay is based on knowing the time of the systic interrupt period
    if we have the interrupt fireing at every 1ms, then to get a 1 second delay, we need to pass 1000 as nTime
*/
void Delay(uint32_t nTime)
{
  TimeDelay=nTime;
  while(TimeDelay !=0);
}


//defieing the systeick interrupt handler here
void SysTick_Handler(void)
{
	if (TimeDelay>0)
	{
		TimeDelay--;//variable is global and volatile
	}
}
