 /*
 * Pixel_LED_Driver.h
 *
 *  Created on: Dec 12, 2024
 *      Author: Joel Imbaud
 */

#ifndef INC_PIXEL_LED_DRIVER_H_
#define INC_PIXEL_LED_DRIVER_H_

#include "stdint.h"
#include "stm32l4xx_hal.h"

// ========================
// Paramètres des LEDs et configuration
// ========================

// Définir le type de LED utilisé (WS2812B ou SK6812)
#define LED_TYPE           LED_TYPE_WS2812B  // Ou LED_TYPE_SK6812

// Constantes pour les signaux PWM
#define PWM_HI             (38)  // Durée du signal "1" logique
#define PWM_LO             (19)  // Durée du signal "0" logique

// Nombre de bits par pixel (3 pour RGB, 4 pour RGBW)
#if LED_TYPE == LED_TYPE_WS2812B
    #define NUM_BPP       (3)  // 3 canaux par LED (R, G, B)
#elif LED_TYPE == LED_TYPE_SK6812
    #define NUM_BPP       (4)  // 4 canaux par LED (R, G, B, W)
#endif

// Nombre total de LEDs sur la bande
#define NUM_PIXELS         (12) // Nombre total de LEDs
#define NUM_BYTES (NUM_BPP * NUM_PIXELS)
#define WR_BUF_LEN (NUM_BPP * 8 * 2)

// ========================
// Paramètres du Timer et du DMA
// ========================
extern TIM_HandleTypeDef htim16;           // Déclaration globale du Timer
extern DMA_HandleTypeDef hdma_tim16_ch1_up;   // Déclaration globale du DMA

// Définir globalement le timer et le canal utilisés
#define HTIM                htim16           // Choisissez le timer (htim1, htim2, etc.) suivant la déclaration globale du Timer
#define DMA_CHANNEL         hdma_tim16_ch1_up   // Choisissez le canal DMA associé suivant la déclaration globale du DMA

#define TIM_CHANNEL         TIM_CHANNEL_1   // Canal utilisé pour le PWM suivant la configuration

// ========================
// Prototypes des fonctions
// ========================

/**
 * @brief  Set the RGB color for a specific LED.
 * @param  index: LED index (0 to NUM_PIXELS-1)
 * @param  r: Red color intensity (0-255)
 * @param  g: Green color intensity (0-255)
 * @param  b: Blue color intensity (0-255)
 * @retval None
 * @note   This function sets the RGB values for a specific LED.
 */
void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief  Set the RGBW color for a specific LED (used for SK6812 LEDs).
 * @param  index: LED index (0 to NUM_PIXELS-1)
 * @param  r: Red color intensity (0-255)
 * @param  g: Green color intensity (0-255)
 * @param  b: Blue color intensity (0-255)
 * @param  w: White color intensity (0-255, for SK6812 only)
 * @retval None
 * @note   This function sets the RGBW values for a specific LED (for RGBW LEDs like SK6812).
 */
void led_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * @brief  Set the same RGB color for all LEDs.
 * @param  r: Red color intensity (0-255)
 * @param  g: Green color intensity (0-255)
 * @param  b: Blue color intensity (0-255)
 * @retval None
 * @note   This function applies the same RGB color to all LEDs in the array.
 */
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief  Set the same RGBW color for all LEDs (used for SK6812 LEDs).
 * @param  r: Red color intensity (0-255)
 * @param  g: Green color intensity (0-255)
 * @param  b: Blue color intensity (0-255)
 * @param  w: White color intensity (0-255, for SK6812 only)
 * @retval None
 * @note   This function applies the same RGBW color to all LEDs (for RGBW LEDs like SK6812).
 */
void led_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * @brief  Update the LEDs with the current color data.
 * @retval None
 * @note   This function sends the current color data to the LEDs using DMA, updating the display.
 */
void led_render(void);

/**
 * @brief  Convert HSL (Hue, Saturation, Lightness) to RGB color format.
 * @param  h: Hue (0 to 255)
 * @param  s: Saturation (0 to 255)
 * @param  l: Lightness (0 to 255)
 * @retval RGB color value as a 32-bit integer (0xRRGGBB)
 * @note   This function converts HSL values to the RGB color format used by the LEDs.
 */
uint32_t hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l);

/**
 * @brief  Perform the rainbow effect across the LEDs.
 * @param  duration_seconds: Duration of the effect in seconds.
 * @param  num_pixels: Number of pixels (LEDs) in the strip.
 * @param  number_color_per_led: Number of color variations per LED.
 * @retval None
 * @note   This function creates a rainbow effect that scrolls through the LEDs.
 *         The effect runs for the specified duration, updating the LED colors accordingly.
 */
void rainbow_effect(uint16_t duration_seconds, uint8_t num_pixels, uint8_t number_color_per_led);



#endif /* INC_PIXEL_LED_DRIVER_H_ */
