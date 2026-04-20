#include "co2.h"
#include "usart.h"
#include "stdint.h"

// CO2传感器数据缓冲区
uint8_t co2_rx_buffer[9];
uint8_t co2_rx_index = 0;
float co2_concentration = 0.0f;

// 初始化CO2传感器
void CO2_Init(void)
{
    // 启动串口2接收中断
    HAL_UART_Receive_IT(&huart2, &co2_rx_buffer[co2_rx_index], 1);
}

// 处理CO2传感器数据
void CO2_ProcessData(void)
{
    // 检查数据格式是否正确（模块地址为0x2C 0xE4）
    if (co2_rx_buffer[0] == 0x2C && co2_rx_buffer[1] == 0xE4)
    {
        // 计算校验和
        uint8_t checksum = 0;
        for (uint8_t i = 0; i < 8; i++)
        {
            checksum += co2_rx_buffer[i];
        }
        
        // 验证校验和
        if (checksum == co2_rx_buffer[8])
        {
            // 计算CO2浓度：(高字节*256 + 低字节) * 0.001
            uint16_t co2_raw = (co2_rx_buffer[6] << 8) | co2_rx_buffer[7];
            co2_concentration = co2_raw * 0.001f;
        }
    }
    
    // 重置接收索引
    co2_rx_index = 0;
    
    // 重新启动串口接收中断
    HAL_UART_Receive_IT(&huart2, &co2_rx_buffer[co2_rx_index], 1);
}

// 获取CO2浓度值
float CO2_GetConcentration(void)
{
    return co2_concentration;
}

// 串口2接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        co2_rx_index++;
        
        // 当接收到9字节数据时，处理数据
        if (co2_rx_index >= 9)
        {
            CO2_ProcessData();
        }
        else
        {
            // 继续接收下一个字节
            HAL_UART_Receive_IT(&huart2, &co2_rx_buffer[co2_rx_index], 1);
        }
    }
}
