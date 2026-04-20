#include "settings.h"
#include "stm32f1xx_hal_flash.h"

static Settings_t g_settings;
static SettingMode_t g_setting_mode = SETTING_MODE_NORMAL;

static uint32_t Settings_CalculateCRC(Settings_t* settings)
{
    uint32_t crc = 0;
    uint8_t* data = (uint8_t*)settings;
    for (int i = 0; i < sizeof(Settings_t) - sizeof(uint32_t); i++) {
        crc ^= data[i];
        crc = (crc << 1) | (crc >> 31);
    }
    return crc;
}

void Settings_SetDefaults(void)
{
    g_settings.magic = SETTINGS_MAGIC_NUM;
    g_settings.temp_threshold = 35;
    g_settings.humidity_threshold = 80;
    g_settings.co2_threshold = 1000.0f;
    g_settings.mq5_threshold = 500;
    g_settings.crc = Settings_CalculateCRC(&g_settings);
}

void Settings_Load(void)
{
    Settings_t* flash_settings = (Settings_t*)SETTINGS_FLASH_ADDR;
    
    if (flash_settings->magic == SETTINGS_MAGIC_NUM) {
        uint32_t crc = Settings_CalculateCRC(flash_settings);
        if (crc == flash_settings->crc) {
            g_settings = *flash_settings;
            return;
        }
    }
    
    Settings_SetDefaults();
}

void Settings_Save(void)
{
    g_settings.crc = Settings_CalculateCRC(&g_settings);
    
    HAL_FLASH_Unlock();
    
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t pageError = 0;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = SETTINGS_FLASH_ADDR;
    eraseInit.NbPages = 1;
    HAL_FLASHEx_Erase(&eraseInit, &pageError);
    
    uint32_t* data = (uint32_t*)&g_settings;
    uint32_t word_count = (sizeof(Settings_t) + 3) / 4;
    
    for (uint32_t i = 0; i < word_count; i++) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SETTINGS_FLASH_ADDR + i * 4, data[i]);
    }
    
    HAL_FLASH_Lock();
}

void Settings_Init(void)
{
    Settings_Load();
}

Settings_t* Settings_Get(void)
{
    return &g_settings;
}

void SettingMode_Set(SettingMode_t mode)
{
    g_setting_mode = mode;
}

SettingMode_t SettingMode_Get(void)
{
    return g_setting_mode;
}

void SettingMode_Next(void)
{
    if (g_setting_mode < SETTING_MODE_EXIT) {
        g_setting_mode++;
    } else {
        g_setting_mode = SETTING_MODE_NORMAL;
    }
}

void SettingMode_Prev(void)
{
    if (g_setting_mode > SETTING_MODE_NORMAL) {
        g_setting_mode--;
    } else {
        g_setting_mode = SETTING_MODE_EXIT;
    }
}

void SettingValue_Increase(void)
{
    switch (g_setting_mode) {
        case SETTING_MODE_TEMP:
            if (g_settings.temp_threshold < 50) g_settings.temp_threshold++;
            break;
        case SETTING_MODE_HUMIDITY:
            if (g_settings.humidity_threshold < 90) g_settings.humidity_threshold += 10;
            break;
        case SETTING_MODE_CO2:
            if (g_settings.co2_threshold < 4200.0f) g_settings.co2_threshold += 100.0f;
            break;
        case SETTING_MODE_MQ5:
            if (g_settings.mq5_threshold < 4200) g_settings.mq5_threshold += 100;
            break;
        default:
            break;
    }
}

void SettingValue_Decrease(void)
{
    switch (g_setting_mode) {
        case SETTING_MODE_TEMP:
            if (g_settings.temp_threshold > 0) g_settings.temp_threshold--;
            break;
        case SETTING_MODE_HUMIDITY:
            if (g_settings.humidity_threshold >= 10) g_settings.humidity_threshold -= 10;
            break;
        case SETTING_MODE_CO2:
            if (g_settings.co2_threshold >= 100.0f) g_settings.co2_threshold -= 100.0f;
            break;
        case SETTING_MODE_MQ5:
            if (g_settings.mq5_threshold >= 100) g_settings.mq5_threshold -= 100;
            break;
        default:
            break;
    }
}
