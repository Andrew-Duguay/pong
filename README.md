# STM32-Pong | Real-Time Embedded System

A high-performance, bare-metal implementation of Pong for the **STM32F446**. This project integrates custom peripheral drivers, concurrency primitives, and mathematical optimizations to achieve smooth gameplay on resource-constrained hardware.

---

## Key Engineering Features

### 1. Real-Time Execution & Concurrency
* **NVIC Timer Interrupts:** Utilizes the Nested Vectored Interrupt Controller (NVIC) to drive a consistent 60Hz frame refresh, ensuring deterministic physics and rendering.
* **Race Condition Prevention:** Implements Semaphores and Global Atomic Flags to manage access to the $I^2C$ bus, preventing data corruption between the high-priority timer interrupt and the main game logic.

### 2. High-Performance Optimization
**Power Efficiency:** Hard-coded to operate without the internal FPU (Floating Point Unit), demonstrating a "lean" embedded design capable of running on lower-power silicon.
* * **Fixed-Point Arithmetic:** Replaced standard floating-point operations with fixed-point math for ball trajectory calculations, ensuring smooth motion while maintaining high CPU efficiency.
* * **Trajectory Look-Up Tables (LUTs):** Drastically reduced trigonometric computation overhead by utilizing pre-computed LUTs for physics reflections.

### 3. Dynamic Gameplay Logic
* **Procedural Difficulty:** Implements logic to scale ball velocity and paddle response based on current score and game duration.
* **Resource-Light Rendering:** Optimized OLED range of display updates to include only moving entities (ball/paddles), preserving bus bandwidth.

---

## System Architecture
* **Microcontroller:** STM32F446RE (ARM Cortex-M4) or STM32F401CE (Black Pill)
* **Display:** SSD1306 OLED (via I2C)
* **Input:** Analog 2-axis Joystick (via ADC)
* **Dependencies:** * [STM32 Peripheral Drivers](https://github.com/Andrew-Duguay/stm32drivers.git)
    * [External Component Drivers](https://github.com/Andrew-Duguay/componentDrivers.git)

---

## Setup & Usage
1. Connect the **SSD1306 OLED** to I2C1 (PB8/PB9).
2. Connect the **Analog Joystick** to PA1 (X-Axis) and PA0 (Switch).
3. Flash the binary using STM32CubeProgrammer.
