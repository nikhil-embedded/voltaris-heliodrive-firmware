/**
 * @file    ina219_manager.c
 * @brief   INA219 SOC Manager — Solar Hybrid EV
 * @author  Nikhil Sanjay Nikam
 * @date    2026
 */

#include "ina219_manager.h"

/* ── Solar state ─────────────────────────────────────── */
static uint32_t solar_active_ms = 0;
static uint32_t solar_last_tick = 0;
static int      solar_locked    = 0;
static int      solar_percent   = SOLAR_SIM_START;

/* ── Backup state ────────────────────────────────────── */
static uint32_t backup_start_tick = 0;
static int      backup_sim_active = 0;

/* ── Configure INA219 ────────────────────────────────── */
static void INA219_Configure(void)
{
    uint8_t cfg[3];
    cfg[0] = INA219_REG_CONFIG;
    cfg[1] = 0x3F;
    cfg[2] = 0xFF;
    HAL_I2C_Master_Transmit(&hi2c1,
                             INA219_ADDR_SOLAR,
                             cfg, 3, 100);
    HAL_Delay(10);
}

/* ── Read shunt current in mA ────────────────────────── */
static float INA219_Read_Current_mA(void)
{
    uint8_t reg     = INA219_REG_SHUNT;
    uint8_t data[2] = {0};

    if (HAL_I2C_Master_Transmit(&hi2c1,
                                 INA219_ADDR_SOLAR,
                                 &reg, 1, 100) != HAL_OK)
        return 0.0f;
    HAL_Delay(2);
    if (HAL_I2C_Master_Receive(&hi2c1,
                                INA219_ADDR_SOLAR,
                                data, 2, 100) != HAL_OK)
        return 0.0f;

    int16_t raw    = (int16_t)((data[0] << 8) | data[1]);
    float shunt_mv = (float)raw * 0.01f;
    float current  = shunt_mv / INA219_SHUNT_OHMS;

    /* Use absolute value */
    if (current < 0.0f) current = -current;

    return current;
}

/* ── Public API ──────────────────────────────────────── */
void INA219_Manager_Init(void)
{
    INA219_Configure();
    HAL_Delay(100);

    solar_active_ms   = 0;
    solar_last_tick   = HAL_GetTick();
    solar_locked      = 0;
    solar_percent     = SOLAR_SIM_START;

    backup_sim_active = 0;
    backup_start_tick = 0;
}

void INA219_Reset_Backup_Timer(void)
{
    backup_start_tick = HAL_GetTick();
    backup_sim_active = 1;
}

void INA219_Manager_Update(void)
{
    if (solar_locked)
        return;

    float current = INA219_Read_Current_mA();

    uint32_t now = HAL_GetTick();
    uint32_t dt  = now - solar_last_tick;
    solar_last_tick = now;

    /* Accumulate active time only when motor draws current */
    if (current > MOTOR_CURRENT_THRESHOLD_MA)
        solar_active_ms += dt;

    /* Linear decline SOLAR_SIM_START → SOLAR_SIM_END */
    int range = SOLAR_SIM_START - SOLAR_SIM_END;
    int drop  = (int)(((float)solar_active_ms
                       / (float)SOLAR_SIM_MS)
                       * (float)range);

    int new_percent = SOLAR_SIM_START - drop;

    if (new_percent <= SOLAR_SIM_END)
    {
        new_percent  = SOLAR_SIM_END;
        solar_locked = 1;
    }

    solar_percent = new_percent;
}

int INA219_Get_Solar_Percent(void)
{
    return solar_percent;
}

int INA219_Get_Backup_Percent(void)
{
    if (!backup_sim_active)
        return BACKUP_SIM_START;

    uint32_t elapsed = HAL_GetTick() - backup_start_tick;

    if (elapsed >= BACKUP_SIM_MS)
        return BACKUP_SIM_END;

    int range   = BACKUP_SIM_START - BACKUP_SIM_END;
    int drop    = (int)((float)elapsed
                        / (float)BACKUP_SIM_MS
                        * (float)range);
    int percent = BACKUP_SIM_START - drop;

    if (percent < BACKUP_SIM_END)
        percent = BACKUP_SIM_END;

    return percent;
}
