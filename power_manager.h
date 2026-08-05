/**
 * @file    power_manager.h
 * @brief   Three-State Power Management State Machine
 * @author  Nikhil Sanjay Nikam
 * @date    2026
 *
 * States:
 *   STATE_SOLAR    → Solar battery powers motor (priority 1)
 *   STATE_BACKUP   → Backup battery powers motor (priority 2)
 *   STATE_CHARGING → Both depleted, motor stopped (protective)
 *
 * Thresholds:
 *   SOLAR_START_HYST     = 21% (solar activates above 20%)
 *   SOLAR_STOP_HYST      = 20% (solar deactivates at 20%)
 *   BACKUP_ELIGIBLE_HYST = 52% (backup eligible above 52%)
 *   BACKUP_STOP_HYST     = 20% (backup deactivates at 20%)
 */

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

/* SOC Switching Thresholds */
#define SOLAR_START_HYST       21
#define SOLAR_STOP_HYST        20
#define BACKUP_ELIGIBLE_HYST   52
#define BACKUP_STOP_HYST       20

typedef enum {
    STATE_SOLAR,
    STATE_BACKUP,
    STATE_CHARGING
} PowerState;

typedef struct {
    PowerState current_state;
    PowerState previous_state;
} PowerManager;

void       PowerManager_Init(PowerManager *pm);
void       PowerManager_Update(PowerManager *pm,
                               int solar, int backup);
PowerState PowerManager_GetState(PowerManager *pm);

#endif /* POWER_MANAGER_H */
