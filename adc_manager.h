#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H

#include "adc.h"
#include "main.h"

#define ADC_EMA_ALPHA        0.8f
#define ADC_NUM_SAMPLES      16

#define BAT_VOLTAGE_MAX      12.6f
#define BAT_VOLTAGE_MIN      10.5f
#define VOLTAGE_DIVIDER_S    4.338f
#define VOLTAGE_DIVIDER_B    4.315f
#define ADC_VREF_S           2.851f
#define ADC_VREF_B           2.744f
#define ADC_RESOLUTION       4095.0f

void ADC_Manager_Init(void);
void ADC_Manager_Update(void);
int  ADC_Get_Solar_Percent(void);
int  ADC_Get_Backup_Percent(void);

#endif