# ☀️ Solar Hybrid EV — Smart Power Switching Vehicle

> **Developed by:** Nikhil Sanjay Nikam  
> **Institution:** Vidya Pratishthan's KBIET, Baramati  
> **Department:** Electronics & Telecommunication Engineering  
> **Academic Year:** 2025–26  
> **Guide:** Mr. V.U. Deshmukh

---

## 📌 Project Overview

Solar Hybrid EV is an embedded systems project that demonstrates intelligent, real-time automatic power source management between a solar-charged primary battery and a backup battery. The system uses relay-based switching controlled by an STM32 NUCLEO-G491RE microcontroller, with real-time State of Charge (SOC) monitoring via INA219 I2C current/voltage sensor modules.

The vehicle operates on a **three-state priority-based power management system**:
- **STATE_SOLAR** — Motor runs on solar-charged battery (primary)
- **STATE_BACKUP** — Motor runs on backup battery (secondary)
- **STATE_CHARGING** — Both batteries below threshold, motor stops

---

## 🎯 Key Features

- ✅ Real-time SOC estimation using **Coulomb Counting + Voltage Fusion**
- ✅ INA219 I2C sensor for accurate current & voltage measurement
- ✅ Three-state intelligent power switching state machine
- ✅ 200ms relay dead-time preventing battery short-circuit
- ✅ 16×2 I2C LCD displaying live Mode + Battery % in real time
- ✅ Active LOW relay logic with safe HIGH-default at startup
- ✅ W88 PWM Solar Charge Controller for regulated solar charging
- ✅ Concurrent charge-discharge (solar charges while motor runs)
- ✅ EMA filtering for stable, noise-free voltage readings

---

## 🔧 Hardware Components

| Component | Specification | Role |
|---|---|---|
| STM32 NUCLEO-G491RE | ARM Cortex-M4, 170MHz | Main controller |
| INA219 × 2 | I2C, 26V, 3.2A, 0.1Ω shunt | Current & voltage sensing |
| 12V 1.3Ah SLA Battery × 2 | AP12-1.3, VRLA | Solar primary + Backup |
| Solar Panel | 18V, 10W, Model 1230 | Renewable energy source |
| W88 PWM Charge Controller | 12V/10A | Solar battery charging |
| Hub ME002 Charger | 12V, 1A, Auto-cutoff | Backup battery charging |
| JQC-3FC Relay × 2 | 5V, Active LOW | Power source switching |
| L298N Motor Driver | 12V, 2A | DC motor control |
| Johnson DC Motor | 12V, 30RPM | Vehicle drive |
| 16×2 I2C LCD | PCF8574, 0x27 | Real-time display |
| 5V Powerbank | USB output | MCU independent power |

---

## 🗂️ Repository Structure

```
solar-hybrid-ev/
│
├── firmware/
│   └── Core/
│       ├── Inc/
│       │   ├── ina219_manager.h      ← INA219 sensor driver header
│       │   ├── power_manager.h       ← State machine header
│       │   ├── lcd_i2c.h             ← LCD driver header
│       │   └── main.h                ← HAL main header
│       └── Src/
│           ├── ina219_manager.c      ← SOC fusion algorithm
│           ├── power_manager.c       ← Three-state machine
│           ├── lcd_i2c.c             ← I2C LCD driver
│           └── main.c                ← System integration
│
├── docs/
│   ├── wiring_diagram.png            ← INA219 wiring diagram
│   ├── block_diagram.png             ← System block diagram
│   └── project_report.pdf            ← Full project report
│
├── hardware/
│   └── pin_mapping.md                ← GPIO pin assignments
│
└── README.md
```

---

## ⚡ SOC Estimation Algorithm

```
Fused SOC = 70% × Coulomb_counting + 30% × Voltage_mapping

Where:
  Coulomb counting: mAh_remaining -= I(mA) × dt(hours)
  Voltage mapping:  SOC% = (V_bat - 10.5) / (12.6 - 10.5) × 100

Near full (>90%) or empty (<10%):
  Weights flip → 30% coulomb + 70% voltage (re-anchor)
  Prevents coulomb counting drift at extremes
```

---

## 🔄 State Machine Logic

```
STATE_SOLAR   → Entry: Solar SOC ≥ 82%
                Exit:  Solar SOC ≤ 20%

STATE_BACKUP  → Entry: Solar ≤ 20% AND Backup > 52%
                Exit:  Backup ≤ 20% OR Solar recovers ≥ 82%

STATE_CHARGING → Entry: Both batteries ≤ 20%
                 Exit:  Solar ≥ 82% OR Backup > 52%
```

---

## 📌 GPIO Pin Mapping

| Signal | STM32 Pin | Arduino Label | Function |
|---|---|---|---|
| Solar Relay | PA5 | D13 | Active LOW output |
| Backup Relay | PB0 | A3 | Active LOW output |
| INA219 Solar SCL | PB8 | D15 | I2C1 Clock |
| INA219 Solar SDA | PB9 | D14 | I2C1 Data |
| LCD SCL | PB8 | D15 | I2C1 Clock (shared) |
| LCD SDA | PB9 | D14 | I2C1 Data (shared) |
| MCU Power | VIN | VIN | 5V Powerbank |

---

## 🚀 How to Flash

1. Open **STM32CubeIDE**
2. Create new STM32 project for NUCLEO-G491RE
3. Configure I2C1 on PB8/PB9 in CubeMX
4. Configure PA5, PB0 as GPIO Output (High default)
5. Copy all files from `firmware/Core/Inc/` and `firmware/Core/Src/`
6. Build → Flash via ST-LINK

---

## 📊 System Performance

| Parameter | Value |
|---|---|
| SOC Update Rate | 1 second |
| Relay Dead-time | 200ms |
| INA219 Voltage Resolution | 4mV LSB |
| INA219 Current Resolution | 0.8mA |
| Battery Capacity | 1300mAh each |
| Motor Runtime (Solar only) | ~2 hours |
| Motor Runtime (Backup only) | ~1 hour |

---

## 📄 License

This project is developed and owned by **Nikhil Sanjay Nikam** as an individual personal project.  
© 2026 Nikhil Sanjay Nikam. All rights reserved.

---

## 👤 Author

**Nikhil Sanjay Nikam**  
Electronics & Telecommunication Engineering  
Vidya Pratishthan's KBIET, Baramati  
Academic Year 2025–26
