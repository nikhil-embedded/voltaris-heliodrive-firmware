#include "adc_manager.h"

static float ema_solar         = -1.0f;
static float ema_backup        = -1.0f;
static int solar_display_last  = 0;
static int backup_display_last = 0;

uint16_t debug_solar_raw  = 0;
uint16_t debug_backup_raw = 0;

static uint16_t ADC_Read_Single(ADC_HandleTypeDef *hadc,
                                 uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel      = channel;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;

    HAL_ADC_Stop(hadc);
    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
        return 2048;
    if (HAL_ADC_Start(hadc) != HAL_OK)
        return 2048;
    if (HAL_ADC_PollForConversion(hadc, 100) != HAL_OK)
    {
        HAL_ADC_Stop(hadc);
        return 2048;
    }
    uint16_t val = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return val;
}

static uint16_t ADC_Read_Average(ADC_HandleTypeDef *hadc,
                                  uint32_t channel)
{
    uint32_t sum = 0;
    for (int i = 0; i < ADC_NUM_SAMPLES; i++)
    {
        sum += ADC_Read_Single(hadc, channel);
        HAL_Delay(2);
    }
    return (uint16_t)(sum / ADC_NUM_SAMPLES);
}

static int Voltage_To_Percent(float voltage)
{
    if (voltage >= BAT_VOLTAGE_MAX) return 100;
    if (voltage <= BAT_VOLTAGE_MIN) return 0;
    return (int)(((voltage - BAT_VOLTAGE_MIN) /
                  (BAT_VOLTAGE_MAX - BAT_VOLTAGE_MIN)) * 100.0f);
}

void ADC_Manager_Init(void)
{
    ema_solar  = 11.5f;
    ema_backup = 11.5f;
    solar_display_last  = 50;
    backup_display_last = 50;
}

void ADC_Manager_Update(void)
{
    uint16_t solar_raw  = ADC_Read_Average(&hadc2, ADC_CHANNEL_17);
    uint16_t backup_raw = ADC_Read_Average(&hadc1, ADC_CHANNEL_2);

    debug_solar_raw  = solar_raw;
    debug_backup_raw = backup_raw;

    float solar_v  = ((solar_raw  * ADC_VREF_S) / ADC_RESOLUTION)
                     * VOLTAGE_DIVIDER_S;
    float backup_v = ((backup_raw * ADC_VREF_B) / ADC_RESOLUTION)
                     * VOLTAGE_DIVIDER_B;

    ema_solar  = (ADC_EMA_ALPHA * solar_v)
               + ((1.0f - ADC_EMA_ALPHA) * ema_solar);
    ema_backup = (ADC_EMA_ALPHA * backup_v)
               + ((1.0f - ADC_EMA_ALPHA) * ema_backup);

    solar_display_last  = Voltage_To_Percent(ema_solar);
    backup_display_last = Voltage_To_Percent(ema_backup);
}

int ADC_Get_Solar_Percent(void)  { return solar_display_last;  }
int ADC_Get_Backup_Percent(void) { return backup_display_last; }