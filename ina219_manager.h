/**
 * @file    ina219_manager.h
 * @brief   INA219 SOC Manager — Solar Hybrid EV
 * @author  Nikhil Sanjay Nikam
 * @date    2026
 *
 * SOC Fusion Algorithm:
 *   Fused SOC = 70% Coulomb Counting + 30% Voltage Mapping
 *   Near full/empty: 30% Coulomb + 70% Voltage (re-anchor)
 *
 * Solar  battery: Real INA219 current sensing
 * Backup battery: Timer-based proxy simulation
 */

#ifndef INA219_MANAGER_H
#define INA219_MANAGER_H

#include "i2c.h"
#include "main.h"

/* I2C Address */
#define INA219_ADDR_SOLAR    (0x40 << 1)   /* A0=GND, A1=GND */

/* INA219 Registers */
#define INA219_REG_CONFIG    0x00
#define INA219_REG_SHUNT     0x01
#define INA219_REG_VOLTAGE   0x02

/* Hardware constants */
#define INA219_SHUNT_OHMS    0.1f          /* R100 shunt resistor */

/* Motor detection threshold */
#define MOTOR_CURRENT_THRESHOLD_MA   50.0f

/* Solar proxy: 100% → 20% over 2 hours MOTOR-ACTIVE time */
#define SOLAR_SIM_START   100
#define SOLAR_SIM_END     20
#define SOLAR_SIM_MS      7200000UL        /* 2 hours */

/* Backup proxy: 100% → 20% over 60 mins wall-clock time */
#define BACKUP_SIM_START  100
#define BACKUP_SIM_END    20
#define BACKUP_SIM_MS     3600000UL        /* 60 minutes */

/* Public API */
void INA219_Manager_Init(void);
void INA219_Manager_Update(void);
void INA219_Reset_Backup_Timer(void);
int  INA219_Get_Solar_Percent(void);
int  INA219_Get_Backup_Percent(void);

#endif /* INA219_MANAGER_H */
