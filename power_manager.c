/**
 * @file    power_manager.c
 * @brief   Three-State Power Management State Machine
 * @author  Nikhil Sanjay Nikam
 * @date    2026
 */

#include "power_manager.h"

void PowerManager_Init(PowerManager *pm)
{
    pm->current_state  = STATE_CHARGING;
    pm->previous_state = STATE_CHARGING;
}

void PowerManager_Update(PowerManager *pm,
                          int solar, int backup)
{
    pm->previous_state = pm->current_state;

    switch (pm->current_state)
    {
        case STATE_SOLAR:
            if (solar <= SOLAR_STOP_HYST)
            {
                if (backup > BACKUP_ELIGIBLE_HYST)
                    pm->current_state = STATE_BACKUP;
                else
                    pm->current_state = STATE_CHARGING;
            }
            break;

        case STATE_BACKUP:
            if (solar >= SOLAR_START_HYST)
                pm->current_state = STATE_SOLAR;
            else if (backup <= BACKUP_STOP_HYST)
                pm->current_state = STATE_CHARGING;
            break;

        case STATE_CHARGING:
            if (solar >= SOLAR_START_HYST)
                pm->current_state = STATE_SOLAR;
            else if (backup > BACKUP_ELIGIBLE_HYST)
                pm->current_state = STATE_BACKUP;
            break;

        default:
            pm->current_state = STATE_CHARGING;
            break;
    }
}

PowerState PowerManager_GetState(PowerManager *pm)
{
    return pm->current_state;
}
