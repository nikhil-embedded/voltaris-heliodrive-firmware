#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "gpio.h"
#include "lcd_i2c.h"
#include "power_manager.h"
#include "adc_manager.h"
#include <stdio.h>
#include <string.h>

void SystemClock_Config(void);
void Error_Handler(void);
void Relay_Init(void);
void Relay_SetState(PowerState state);
void Display_Update(PowerState state, int solar, int backup);

#define SOLAR_RELAY_PIN    GPIO_PIN_5
#define SOLAR_RELAY_PORT   GPIOA
#define BACKUP_RELAY_PIN   GPIO_PIN_0
#define BACKUP_RELAY_PORT  GPIOB

PowerManager pm;
PowerState last_relay_state = STATE_CHARGING;

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_Delay(10);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_Delay(10);

    Relay_Init();
    last_relay_state = STATE_CHARGING;

    HAL_Delay(200);
    LCD_Init();
    HAL_Delay(100);
    LCD_Clear();

    LCD_Set_Cursor(0, 0);
    LCD_Send_String("  HelioDrive 2.0");
    LCD_Set_Cursor(1, 0);
    LCD_Send_String("  Initializing..");
    HAL_Delay(2000);
    LCD_Clear();

    ADC_Manager_Init();
    PowerManager_Init(&pm);

    while (1)
    {
        ADC_Manager_Update();
        int solar  = ADC_Get_Solar_Percent();
        int backup = ADC_Get_Backup_Percent();

        PowerManager_Update(&pm, solar, backup);

        if (pm.current_state != last_relay_state)
        {
            Relay_SetState(pm.current_state);
            last_relay_state = pm.current_state;
        }

        Display_Update(pm.current_state, solar, backup);

        HAL_Delay(1000);
    }
}

void Relay_Init(void)
{
    HAL_GPIO_WritePin(SOLAR_RELAY_PORT,  SOLAR_RELAY_PIN,  GPIO_PIN_SET);
    HAL_GPIO_WritePin(BACKUP_RELAY_PORT, BACKUP_RELAY_PIN, GPIO_PIN_SET);
}

void Relay_SetState(PowerState state)
{
    HAL_GPIO_WritePin(SOLAR_RELAY_PORT,  SOLAR_RELAY_PIN,  GPIO_PIN_SET);
    HAL_GPIO_WritePin(BACKUP_RELAY_PORT, BACKUP_RELAY_PIN, GPIO_PIN_SET);
    HAL_Delay(200);

    switch (state)
    {
        case STATE_SOLAR:
            HAL_GPIO_WritePin(SOLAR_RELAY_PORT,
                              SOLAR_RELAY_PIN, GPIO_PIN_RESET);
            break;
        case STATE_BACKUP:
            HAL_GPIO_WritePin(BACKUP_RELAY_PORT,
                              BACKUP_RELAY_PIN, GPIO_PIN_RESET);
            break;
        case STATE_CHARGING:
        default:
            break;
    }
}

void Display_Update(PowerState state, int solar, int backup)
{
    char line1[17];
    char line2[17];

    switch (state)
    {
        case STATE_SOLAR:
            snprintf(line1, sizeof(line1), "Mode: SOLAR     ");
            break;
        case STATE_BACKUP:
            snprintf(line1, sizeof(line1), "Mode: BACKUP    ");
            break;
        case STATE_CHARGING:
            snprintf(line1, sizeof(line1), "Mode: CHARGING  ");
            break;
        default:
            snprintf(line1, sizeof(line1), "Mode: UNKNOWN   ");
            break;
    }

    snprintf(line2, sizeof(line2), "S:%3d%%  B:%3d%%  ",
             solar, backup);

    LCD_Set_Cursor(0, 0);
    LCD_Send_String(line1);
    LCD_Set_Cursor(1, 0);
    LCD_Send_String(line2);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV4;
    RCC_OscInitStruct.PLL.PLLN            = 85;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK  |
                                       RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_PCLK1  |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_8) != HAL_OK)
        Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif