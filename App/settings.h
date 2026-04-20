#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "main.h"
#include <stdint.h>

#define SETTINGS_FLASH_ADDR  0x0801F800
#define SETTINGS_MAGIC_NUM   0x12345678

typedef struct {
    uint32_t magic;
    int16_t  temp_threshold;
    uint16_t humidity_threshold;
    float    co2_threshold;
    uint16_t mq5_threshold;
    uint32_t crc;
} Settings_t;

typedef enum {
    SETTING_MODE_NORMAL = 0,
    SETTING_MODE_TEMP,
    SETTING_MODE_HUMIDITY,
    SETTING_MODE_CO2,
    SETTING_MODE_MQ5,
    SETTING_MODE_EXIT
} SettingMode_t;

void Settings_Init(void);
void Settings_Load(void);
void Settings_Save(void);
void Settings_SetDefaults(void);

Settings_t* Settings_Get(void);

void SettingMode_Set(SettingMode_t mode);
SettingMode_t SettingMode_Get(void);
void SettingMode_Next(void);
void SettingMode_Prev(void);

void SettingValue_Increase(void);
void SettingValue_Decrease(void);

#endif
