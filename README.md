# ESP32 SPWM Three-Phase Inverter

This project implements a **24 V three-phase inverter** controlled by an **ESP32**. The inverter generates variable-frequency three-phase SPWM signals from **1 Hz to 50 Hz** using a potentiometer input. Three **IR2101 gate drivers** are used to drive a six-MOSFET bridge built with **IRFZ44 MOSFETs**.

> **For the complete design details, calculations, circuit explanation, and oscilloscope results, refer to the full project report in `docs/report.pdf`.**

## Features

- ESP32-based three-phase SPWM generation
- 24 V DC input inverter bridge
- IR2101 high-side/low-side gate drivers
- Six IRFZ44 MOSFET power stage
- Potentiometer-based frequency control from 1 Hz to 50 Hz
- Complementary PWM signals with dead time
- Bootstrap high-side gate driving
- Datasheet-based gate resistor, bootstrap capacitor, dead-time, and heatsink selection
- Oscilloscope-verified PWM and inverter output waveforms

## Project Structure

```text
esp32-spwm-three-phase-inverter/
│
├── firmware/
│   └── three_phase_inverter_spwm.ino
│
├── docs/
│   └── report.pdf
│
├── images/
│   ├── schematic.png
│   ├── block_diagram.png
│   └── oscilloscope_results.png
│
├── README.md
└── LICENSE
```

## Hardware Used

| Component | Description |
|---|---|
| ESP32 | Main SPWM controller |
| IR2101 | High-side and low-side MOSFET gate driver |
| IRFZ44 | N-channel power MOSFET |
| Potentiometer | Output frequency control |
| Bootstrap capacitor | 470 nF |
| Gate resistor | 75 Ω |
| DC input | 24 V |
| Gate-drive supply | 12 V |

## Control Method

The ESP32 generates three sinusoidal PWM duty cycles shifted by 120°:

```text
Phase A: sin(theta)
Phase B: sin(theta - 120°)
Phase C: sin(theta - 240°)
```

The MCPWM peripheral is used to generate complementary PWM pairs with dead time for each inverter phase leg. The potentiometer is read through the ESP32 ADC and mapped to the output frequency range.

## Main Specifications

| Parameter | Value |
|---|---|
| DC bus voltage | 24 V |
| PWM carrier frequency | 10 kHz |
| Output frequency range | 1 Hz to 50 Hz |
| Modulation index | 0.85 |
| Gate-drive voltage | 12 V |
| Bootstrap capacitor | 470 nF |
| Gate resistor | 75 Ω |

## Results

Oscilloscope measurements confirmed:

- Complementary PWM generation from the ESP32
- Dead-time behavior between high-side and low-side signals
- Variable output frequency using the potentiometer
- Three-phase inverter switching behavior

## Documentation

📌 **See the full report for the complete technical documentation:**

- Circuit schematic
- Hardware design explanation
- ESP32 MCPWM configuration
- SPWM equations
- Component calculations
- Thermal design
- Oscilloscope measurements

Report file:

```text
docs/report.pdf
```

## Notes

This project was developed for a microcontroller applications lab. The design is intended for educational and experimental use. For practical motor-drive applications, additional protection circuits such as overcurrent protection, current sensing, better PCB layout, and stronger decoupling are recommended.

## References

- IR2101 High and Low Side Driver Datasheet
- IRFZ44 Power MOSFET Datasheet
- ESP32 MCPWM Documentation
- CUI Devices Thermal Management Guide
