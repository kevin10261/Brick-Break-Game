# Breakout Game on Zybo Z7 with 128x32 OLED Display

## Overview
This project implements a classic Breakout (Brick Breaker) game on a **128x32 pixel graphic OLED display** using the **Zybo Z7 board**. The game is developed using **FreeRTOS** and features paddle movement, a bouncing ball, breakable bricks, and a score/lives display.

## Features
- **Paddle Control:** Move the paddle left and right using Zybo Z7 onboard buttons.
- **Ball Physics:** The ball bounces off walls, bricks, and the paddle.
- **Brick Collision:** Bricks disappear when hit, and score increases.
- **Lives System:** The player has 3 lives; losing all results in "Game Over."
- **Keypad Integration (Future Work):** The game includes a keypad interface for potential additional controls.

## Hardware Requirements
- **Zybo Z7 Board**
- **128x32 Pixel Pmod OLED Display**
- **Pmod Keypad (Optional)**

## Software Requirements
- **FreeRTOS**
- **Xilinx SDK**
- **Vivado Design Suite**
- **C Programming Language**

## Setup & Installation
1. **Clone the repository:**
   ```sh
   git clone https://github.com/your-username/breakout-zybo.git
   cd breakout-zybo
   ```
2. **Open Xilinx SDK:**
   - Import the project into Xilinx SDK.
   - Ensure all dependencies (FreeRTOS, Xilinx drivers) are correctly included.
3. **Connect Hardware:**
   - Attach the **Pmod OLED** to the appropriate port on Zybo Z7.
   - Connect the **Pmod Keypad** (if used) to the specified port.
4. **Build & Run:**
   - Compile the project.
   - Program the Zybo Z7 board.
   - Observe the game running on the OLED display.

## Game Controls
- **Left Button:** Move paddle left.
- **Right Button:** Move paddle right.
- **Ball Auto-Movement:** The ball moves automatically and bounces off surfaces.

## Code Structure
- `main.c` – Initializes the hardware and FreeRTOS tasks.
- `OLEDControllerCustom.h` – Custom OLED display functions.
- `pmodkypd.h` – Keypad driver.
- `FreeRTOS tasks` – Handles OLED display updates and game logic.

## Future Improvements
- Add different difficulty levels.
- Implement sound effects (if supported by hardware).
- Expand controls using the Pmod Keypad.

## Acknowledgments
- Xilinx for the Zybo Z7 development board.
- Digilent for the Pmod OLED and Keypad modules.
- FreeRTOS community for real-time OS support.

## License
This project is open-source under the **MIT License**. Feel free to modify and improve it!

