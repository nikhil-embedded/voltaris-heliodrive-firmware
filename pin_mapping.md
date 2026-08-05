# GPIO Pin Mapping — Solar Hybrid EV

## STM32 NUCLEO-G491RE Pin Assignment

| Signal | STM32 Pin | Board Label | Connector | Mode | Notes |
|---|---|---|---|---|---|
| Solar Relay | PA5 | D13 | CN10 | GPIO Output HIGH | Active LOW — HIGH=OFF, LOW=ON |
| Backup Relay | PB0 | A3 | CN7 | GPIO Output HIGH | Active LOW — HIGH=OFF, LOW=ON |
| INA219 SCL | PB8 | D15 | CN10 | I2C1_SCL | Shared with LCD |
| INA219 SDA | PB9 | D14 | CN10 | I2C1_SDA | Shared with LCD |
| LCD SCL | PB8 | D15 | CN10 | I2C1_SCL | Shared with INA219 |
| LCD SDA | PB9 | D14 | CN10 | I2C1_SDA | Shared with INA219 |
| MCU Power | VIN | VIN | CN7 | 5V Input | 5V Powerbank via USB |
| Common GND | GND | GND | CN7 | Ground | All grounds tied here |

---

## I2C Bus Devices (PB8/PB9)

| Device | I2C Address | Notes |
|---|---|---|
| INA219 Solar | 0x40 | A0=GND, A1=GND |
| LCD 16x2 | 0x27 | Fixed address PCF8574 |

---

## Relay Wiring

```
Battery 1 (+) → INA219 VIN+ → INA219 VIN- → Relay 1 COM
                                               Relay 1 NO → L298N 12V input

Battery 2 (+) → Relay 2 COM
                Relay 2 NO  → L298N 12V input

Both relay outputs → L298N 12V
L298N GND → Common GND
L298N OUT → Motor
```

---

## INA219 Wiring

```
INA219 Solar:
  VCC  → STM32 3.3V
  GND  → Common GND
  SCL  → PB8 (D15)
  SDA  → PB9 (D14)
  A0   → GND
  A1   → GND
  VIN+ → Battery 1 (+) positive
  VIN- → Relay 1 NO terminal
```

---

## L298N Motor Driver

```
IN1 → 5V (always HIGH — forward rotation)
IN2 → GND (always LOW — forward rotation)
12V → From active relay output
GND → Common GND
OUT → Johnson DC Motor terminals
```

---

*Developed by Nikhil Sanjay Nikam*
