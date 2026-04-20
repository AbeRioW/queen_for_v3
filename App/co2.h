#ifndef __CO2_H
#define __CO2_H

#include "stdint.h"

extern uint8_t co2_rx_buffer[9];
extern uint8_t co2_rx_index;

void CO2_Init(void);
void CO2_ProcessData(void);
float CO2_GetConcentration(void);

#endif
