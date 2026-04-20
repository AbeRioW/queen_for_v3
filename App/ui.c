#include "ui.h"
#include "co2.h"
#include "settings.h"
#include "mqtt_publisher.h"

void main_ui(void)
{
    DHT11_Data_t dht_data;
    char display_buf[32],display_buf1[32], display_buf2[32], display_buf3[32];
    char temp_buf[16];
    
    if(DHT11_READ_DATA(&dht_data) == 0)
    {
        sprintf((char*)display_buf, "Temp: %d.%dC", dht_data.temp_int, dht_data.temp_dec);
        sprintf((char*)display_buf1, "Humidity: %d.%d%%",dht_data.humidity_int, dht_data.humidity_dec);
        OLED_ShowString(0, 0, (uint8_t*)display_buf, 8, 1);
        OLED_ShowString(0, 8, (uint8_t*)display_buf1, 8, 1);
        
        sprintf(temp_buf, "%d.%d", dht_data.temp_int, dht_data.temp_dec);
        MQTT_Publish_temp(temp_buf);
			
			  sprintf(temp_buf, "%d.%d", dht_data.humidity_int, dht_data.humidity_dec);
        MQTT_Publish_humidity(temp_buf);
    }
    
    float co2_val = CO2_GetConcentration();
    sprintf((char*)display_buf2, "CO2: %.3f mg/m3", co2_val);
    OLED_ShowString(0, 16, (uint8_t*)display_buf2, 8, 1);
//    if(co2_val>0)
//		{
//    int co2_int = (int)co2_val;
//    int co2_dec = (int)((co2_val - co2_int) * 10);
//    if(co2_dec < 0) co2_dec = -co2_dec;
//    sprintf(temp_buf, "%d.%d", co2_int, co2_dec);
//    MQTT_Publish_co2(temp_buf);
//		}
    uint32_t mq5_value = ADC1_ReadValue();
  //  sprintf((char*)display_buf3, "MQ5: %u", (unsigned int)mq5_value);
		    sprintf((char*)display_buf3, "MQ5: %d", mq5_value);
    OLED_ShowString(0, 24, (uint8_t*)display_buf3, 8, 1);
    
    OLED_Refresh();
		sprintf(temp_buf, "%u", (unsigned int)mq5_value);
    MQTT_Publish_mq5(temp_buf);
}

void setting_ui(void)
{
    char display_buf[32];
    Settings_t* settings = Settings_Get();
    SettingMode_t mode = SettingMode_Get();
    
    OLED_ShowString(0, 0, (uint8_t*)"== Setting Mode ==", 8, 1);
    
    sprintf((char*)display_buf, "%sTemp TH: %dC", (mode == SETTING_MODE_TEMP) ? ">" : " ", settings->temp_threshold);
    OLED_ShowString(0, 8, (uint8_t*)display_buf, 8, 1);
    
    sprintf((char*)display_buf, "%sHumidity: %d%%", (mode == SETTING_MODE_HUMIDITY) ? ">" : " ", settings->humidity_threshold);
    OLED_ShowString(0, 16, (uint8_t*)display_buf, 8, 1);
    
    sprintf((char*)display_buf, "%sCO2: %.0f", (mode == SETTING_MODE_CO2) ? ">" : " ", settings->co2_threshold);
    OLED_ShowString(0, 24, (uint8_t*)display_buf, 8, 1);
    
    sprintf((char*)display_buf, "%sMQ5: %u", (mode == SETTING_MODE_MQ5) ? ">" : " ", settings->mq5_threshold);
    OLED_ShowString(0, 32, (uint8_t*)display_buf, 8, 1);
    
    sprintf((char*)display_buf, "%sExit & Save", (mode == SETTING_MODE_EXIT) ? ">" : " ");
    OLED_ShowString(0, 40, (uint8_t*)display_buf, 8, 1);
    
    OLED_Refresh();
}
