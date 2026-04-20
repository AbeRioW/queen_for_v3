#include "co2.h"
#include "usart.h"
#include "stdint.h"


uint8_t co2_rx_buffer[9];
uint8_t co2_rx_index = 0;
float co2_concentration = 0.0f;


void CO2_Init(void)
{

    HAL_UART_Receive_IT(&huart2, &co2_rx_buffer[co2_rx_index], 1);
}


void CO2_ProcessData(void)
{

    if (co2_rx_buffer[0] == 0x2C && co2_rx_buffer[1] == 0xE4)
    {

        uint8_t checksum = 0;
        for (uint8_t i = 0; i < 8; i++)
        {
            checksum += co2_rx_buffer[i];
        }

        if (checksum == co2_rx_buffer[8])
        {

            uint16_t co2_raw = (co2_rx_buffer[6] << 8) | co2_rx_buffer[7];
            co2_concentration = co2_raw * 0.001f;
        }
    }
        // ���ý�������
    co2_rx_index = 0;
    

    HAL_UART_Receive_IT(&huart2, &co2_rx_buffer[co2_rx_index], 1);
}


float CO2_GetConcentration(void)
{
    return co2_concentration;
}


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart == &huart2)
//    {
//        co2_rx_index++;
//        
//        if (co2_rx_index >= 9)
//        {
//            CO2_ProcessData();
//        }
//        else
//        {
//            
//            HAL_UART_Receive_IT(&huart2, &co2_rx_buffer[co2_rx_index], 1);
//        }
//    }
//}
