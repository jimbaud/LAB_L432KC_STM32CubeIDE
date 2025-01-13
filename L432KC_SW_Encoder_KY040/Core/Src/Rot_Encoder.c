#include "Rot_Encoder.h"

/* Variables globales */
static volatile uint16_t encoder_count = 0; /**< Compteur de l'encodeur */
static volatile int encoder_direction = 1;  /**< Direction de rotation de l'encodeur */
volatile uint8_t encoder_flag = 0;   /**< Drapeau pour indiquer un changement de l'encodeur */

static uint32_t last_button_press_time = 0; /**< Pour gérer le rebond du bouton */
char uart_message[100];
const char clear_screen[] = "\x1B[2J\x1B[H";

/**
 * @brief Callback pour la capture d'entrée de l'encodeur.
 * @param htim Pointeur vers le gestionnaire de minuterie.
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        encoder_direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(htim) ? -1 : 1;
        encoder_count += encoder_direction;
        encoder_flag = 1;

        HAL_UART_Transmit(&huart2, (uint8_t*)clear_screen, sizeof(clear_screen) - 1, HAL_MAX_DELAY);
        snprintf(uart_message, sizeof(uart_message), "Valeur encodeur : %d\n", encoder_count);
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_message, strlen(uart_message), HAL_MAX_DELAY);
    }
}

/**
 * @brief Callback pour la gestion des interruptions GPIO (bouton poussoir).
 * @param GPIO_Pin Pin qui a généré l'interruption.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == Encoder_SW_D0_Pin && (HAL_GetTick() - last_button_press_time > 50)) {
        last_button_press_time = HAL_GetTick();
        encoder_flag = 1;

        snprintf(uart_message, sizeof(uart_message), "Bouton poussoir pressé.\n");
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_message, strlen(uart_message), HAL_MAX_DELAY);
    }
}
