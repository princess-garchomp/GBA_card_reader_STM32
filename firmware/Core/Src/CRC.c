#include "CRC.h"

void init_CRC()
{
	RCC->AHBENR |= RCC_AHBENR_CRCEN;
	CRC->CR  |= CRC_CR_RESET; //clear the 8bit dta value in the CRC
}
uint8_t get_CRC_data_value()
{
	return CRC->DR;//when reading the register it give the current claculation
}
void clear_CRC_value()
{
	CRC->CR  |= CRC_CR_RESET;//clear the 8bit dta value in the CRC
}
void update_CRC_data_value(uint32_t input)
{
	CRC->DR = input;//when writing to the register it inputs new data into the CRC
}
