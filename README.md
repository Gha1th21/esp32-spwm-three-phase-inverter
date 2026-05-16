from pathlib import Path

readme = """# ESP32 SPWM Three-Phase Inverter

This project implements a **24 V three-phase inverter** controlled by an **ESP32**. The inverter generates variable-frequency three-phase SPWM signals from **1 Hz to 50 Hz** using a potentiometer input. Three **IR2101 gate drivers** are used to drive a six-MOSFET bridge built with **IRFZ44 MOSFETs**.

The full design, calculations, circuit explanation, and oscilloscope results are documented in the project report.

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
