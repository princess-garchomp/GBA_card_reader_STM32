#ifndef CRC_H
#define CRC_H

#include "stm32f1xx.h"

void init_CRC();
uint8_t get_CRC_data_value();
void clear_CRC_value();
void update_CRC_data_value(uint32_t input);

#endif
