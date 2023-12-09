# Paper Folding Machine

This repository contains the Arduino code for a paper folding machine. The code is designed to control a stepper motor and various digital outputs to facilitate the paper folding process.

## Features

- Stepper motor control for precise paper folding.
- Dynamic adjustment of maximum speed using a potentiometer.
- Digital outputs control based on specific step ranges.

## Getting Started

### Prerequisites

- Arduino IDE installed.
- [AccelStepper library](https://www.airspayce.com/mikem/arduino/AccelStepper/) installed in the Arduino IDE.

### Hardware Setup

1. Connect the stepper motor to the specified pins (`STEP_PIN` and `DIR_PIN`).
2. Connect buttons to pins for enable/disable (`CONT_PIN`), reset (`RESET_PIN`), start/stop (`START_STOP_PIN`), and speed control (`SPEED_PIN`).
3. Connect digital outputs to pins D6, D7, D11, and D12 as specified.

