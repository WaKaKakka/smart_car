# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

STM32F103ZET6 embedded project for a motorized car. Uses STM32CubeMX for peripheral initialization code generation, with custom driver modules for motor control, encoder reading, and OLED display.

## Build Commands

```bash
# Configure (Debug preset)
cmake --preset Debug

# Build
cmake --build build/Debug

# Clean
cmake --build build/Debug --target clean
```

The project uses Ninja generator with `arm-none-eabi-gcc` toolchain. The toolchain file is at `cmake/gcc-arm-none-eabi.cmake`.

## Architecture

### Code Generation Boundary

STM32CubeMX generates code into `Core/` and `Drivers/`. **Do not edit files in `Drivers/`**. In `Core/Src/`, only modify code between `USER CODE BEGIN` and `USER CODE END` markers — CubeMX will overwrite anything outside these blocks on regeneration.

### Peripheral Allocation (from car2.ioc)

| Peripheral | Purpose | Pins |
|------------|---------|------|
| TIM1 CH1 | PWM output for motor | PE9 |
| TIM2 CH1/CH2 | Encoder interface (TI12 mode) | PA0, PA1 |
| TIM3 | Periodic interrupt (1s interval for OLED update) | Internal |
| I2C1 (Fast mode) | OLED SSD1306 communication | PB6 (SCL), PB7 (SDA) |
| GPIO | Motor direction (TB6612 AIN1/AIN2) | PA4, PA5 |

Clock: HSE 8MHz → PLL ×9 = 72MHz SYSCLK. APB1 at 36MHz, APB2 at 72MHz. Timer clocks all 72MHz (APB timers doubled when APB prescaler ≠ 1).

### Custom Modules (`modules/`)

- **PWM** (`modules/src/PWM.c`): TB6612 motor driver. `TB6612_SetSpeed(motor, speed)` sets direction via GPIO and PWM duty via TIM1. Motor 1 uses AIN1/AIN2 for direction.
- **Encoder** (`modules/src/encoder.c`): `Read_Encoder_TIM2()` reads TIM2 counter with interrupt-safe read-and-clear. Returns signed int16_t.
- **OLED** (`modules/src/oled.c`): SSD1306 128×64 display driver over I2C. Framebuffer-based: call `OLED_NewFrame()` → draw → `OLED_ShowFrame()`. Supports ASCII and UTF-8 Chinese fonts (generated via https://led.baud-dance.com).

### Adding New Module Sources

New `.c` files in `modules/src/` must be added to `target_sources` in the root `CMakeLists.txt` (line ~46). Headers in `modules/inc/` are already on the include path.

## Key Patterns

- **Timer interrupt callback**: `HAL_TIM_PeriodElapsedCallback()` in `main.c` handles TIM3 overflow — sets a flag, does no work in ISR itself.
- **Encoder read pattern**: Read counter, then immediately zero it. Cast to `int16_t` for signed direction.
- **OLED update pattern**: Main loop polls `oled_update_flag`, clears it, reads sensor, formats string, renders frame.

## Development Environment

STM32CubeIDE with clangd for IntelliSense. The `.vscode/settings.json` configures `cube-cmake` and `starm-clangd` from the STM32Cube bundle.
