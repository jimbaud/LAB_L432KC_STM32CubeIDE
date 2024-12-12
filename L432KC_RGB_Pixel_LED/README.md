**# Pixel LED Driver Library**  

### 📚 **Description**
This project provides a comprehensive **Pixel LED Driver Library** for controlling WS2812B and SK6812 addressable LEDs using an **STM32 microcontroller**. It offers multiple functions to set individual LED colors, apply global color changes, render effects like **rainbow animation**, and convert **HSL (Hue, Saturation, Luminosity) to RGB** colors. The library utilizes **PWM with DMA** to efficiently transmit the LED color data stream with minimal CPU usage.  

The code is modular, flexible, and easily configurable. Simply adjust the number of LEDs, the color format (RGB or RGBW), and the timer/DMA configurations in the header file.  

---

### ✨ **Features**
- **Support for WS2812B and SK6812** (selectable using `NUM_BPP` in the header file).  
- **Set individual LED colors** (RGB and RGBW modes).  
- **Set all LEDs to the same color** using a simple function.  
- **Rainbow effect** with configurable duration, number of LEDs, and color spread.  
- **HSL to RGB conversion** for advanced color manipulation.  
- **Hardware-accelerated LED data transmission** using **PWM and DMA** for smooth animations.  
- **Modular, scalable design** — add or remove LEDs easily via configuration.  

---

### 🛠️ **Hardware Requirements**
- **STM32 Microcontroller** (tested on STM32L4).  
- **Addressable LEDs** (WS2812B, SK6812, or compatible).  
- **PWM-capable timer** connected to a GPIO pin.  
- **DMA-enabled microcontroller** to reduce CPU load during color rendering.  

---

### 📁 **File Structure**
```
📂 /Pixel_LED_Driver
   ├── 📄 Pixel_LED_Driver.h      // Header file with configuration and prototypes
   └── 📄 Pixel_LED_Driver.c      // Implementation of all functions
```

---

### ⚙️ **Configuration**
Before compiling, make sure to configure the following parameters in **Pixel_LED_Driver.h**:  
- **NUM_PIXELS** — Number of LEDs on the strip.  
- **NUM_BPP** — Number of color channels per LED (3 for WS2812B, 4 for SK6812).  
- **Timer, DMA, and GPIO** — Configure the `TIM` and `DMA` to match your hardware.  

**Example of configuration in the header file**:
```c
#define NUM_PIXELS         (12) // Number of LEDs
#define NUM_BPP            (3)  // 3 for RGB (WS2812B), 4 for RGBW (SK6812)
#define TIM_CHANNEL        TIM_CHANNEL_1
```

---

### 📘 **Function Descriptions**
#### **1️⃣ led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b)**
> Set the RGB color of a single LED.  
- **Parameters**:  
  - `index` — Index of the LED to modify (0 to NUM_PIXELS-1)  
  - `r`, `g`, `b` — Red, Green, Blue components (0-255)  
- **Example**:  
  ```c
  led_set_RGB(0, 255, 0, 0); // Set LED 0 to red
  ```

#### **2️⃣ led_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w)**
> Set the RGBW color of a single LED (for SK6812 only).  
- **Parameters**:  
  - `index` — Index of the LED to modify (0 to NUM_PIXELS-1)  
  - `r`, `g`, `b`, `w` — Red, Green, Blue, and White components (0-255)  

#### **3️⃣ led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b)**
> Set the same RGB color for all LEDs.  
- **Parameters**:  
  - `r`, `g`, `b` — Red, Green, Blue components (0-255)  
- **Example**:  
  ```c
  led_set_all_RGB(0, 255, 0); // Set all LEDs to green
  ```

#### **4️⃣ led_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w)**
> Set the same RGBW color for all LEDs (for SK6812 only).  
- **Parameters**:  
  - `r`, `g`, `b`, `w` — Red, Green, Blue, and White components (0-255)  

#### **5️⃣ led_render()**
> Transmit the LED color buffer to the LED strip using **PWM & DMA**.  
This function must be called after setting colors to update the LEDs.  

#### **6️⃣ hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l)**
> Convert **HSL (hue, saturation, lightness)** to **RGB**.  
- **Parameters**:  
  - `h` — Hue (0-255)  
  - `s` — Saturation (0-255)  
  - `l` — Lightness (0-255)  
- **Returns**: RGB value as a 32-bit integer (0xRRGGBB).  

#### **7️⃣ rainbow_effect(uint16_t duration_seconds, uint8_t num_pixels, uint8_t color_spread)**
> Display a rainbow animation for a specified duration.  
- **Parameters**:  
  - `duration_seconds` — Duration of the rainbow effect in seconds.  
  - `num_pixels` — Number of LEDs to animate.  
  - `color_spread` — Determines the color spread between LEDs (larger values = wider color gap).  

**Example usage**:
```c
rainbow_effect(10, 12, 15); // 10-second rainbow effect on 12 LEDs
```

---

### 🧪 **Example Code**
Here’s an example of how to use the library to create a rainbow effect:  
```c
#include "Pixel_LED_Driver.h"

int main(void) {
    // Initialize system
    HAL_Init();
    SystemClock_Config();
    MX_TIM1_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    // Initialize the LEDs
    led_set_all_RGB(0, 0, 0); // Turn off all LEDs initially
    led_render();

    // Show rainbow effect for 10 seconds on 12 LEDs
    rainbow_effect(10, 12, 15);
    
    while (1) {
        // Infinite loop (you can add custom logic here)
    }
}
```

---

### ⚠️ **Known Issues & Solutions**
| **Issue**       | **Cause**               | **Solution**             |
|-----------------|-----------------------|--------------------------|
| LEDs don't light up | Incorrect timer settings | Check Timer period, prescaler, and DMA link. |
| Only first LED lights up | Buffer misalignment | Ensure WR_BUF_LEN is calculated correctly. |
| Colors are incorrect | Incorrect HSL to RGB | Verify the HSL conversion logic. |

---

### 📦 **Dependencies**
This project depends on the following STM32 libraries:  
- **HAL Driver** (for DMA, PWM, and GPIO control)  
- **HAL DMA** (for high-speed data transfer)  

---

### 🚀 **How to Use**
1. **Clone the repository**:  
   ```bash
   git clone https://github.com/username/Pixel_LED_Driver.git
   ```

2. **Import the files** into your STM32 project.  
3. **Configure the DMA, TIM, and GPIO** for your hardware in `main.c`.  
4. **Call the LED functions** to control your LED strip.  

---

### 🌈 **Future Improvements**
- Add support for other LED protocols (e.g., APA102).  
- Enhance the HSL to RGB algorithm for more precise transitions.  
- Add more effects (e.g., breathing, flashing).  

---

### 🤝 **Contributions**
We welcome contributions! If you have ideas for optimization, bug fixes, or new features, feel free to create an issue or submit a pull request.  

---

### 📜 **License**
This project is licensed under the **MIT License**. You are free to use, modify, and distribute it for personal or commercial use.  

---

With this comprehensive README, you can create a well-documented GitHub repository that will make it easy for users to understand, configure, and use the **Pixel LED Driver** for STM32 microcontrollers. Let me know if you'd like any adjustments or additions! 🚀