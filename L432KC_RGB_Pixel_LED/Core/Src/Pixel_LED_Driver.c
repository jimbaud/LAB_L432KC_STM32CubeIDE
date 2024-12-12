/*
 * Pixel_LED_Driver.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Joel Imbaud
 */
#include "Pixel_LED_Driver.h"

// Déclaration des buffers pour la gestion des couleurs des LEDs et de l'écriture des données
uint8_t rgb_arr[NUM_BYTES] = {0};  // Tableau de stockage des couleurs des LEDs
uint8_t wr_buf[WR_BUF_LEN] = {0};  // Buffer pour l'écriture des données via DMA
uint_fast8_t wr_buf_p = 0;         // Indice de position du buffer d'écriture

// Fonction pour appliquer une échelle à un nombre sur 8 bits
static inline uint8_t scale8(uint8_t x, uint8_t scale) {
  return ((uint16_t)x * scale) >> 8;  // Retourne le résultat de x multiplié par scale, puis décalé de 8 bits
}

/**
 * @brief  Définir la couleur d'une LED spécifique (format RGB)
 * @param  index: Indice de la LED (0 à NUM_PIXELS-1)
 * @param  r: Intensité du rouge (0-255)
 * @param  g: Intensité du vert (0-255)
 * @param  b: Intensité du bleu (0-255)
 * @retval None
 * @note   Cette fonction définit la couleur d'une LED dans un tableau de LEDs, en utilisant les couleurs RGB.
 */
void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
#if (NUM_BPP == 4)  // SK6812
  rgb_arr[4 * index] = scale8(g, 0xB0);  // Appliquer l'échelle à la couleur verte
  rgb_arr[4 * index + 1] = r;            // Appliquer la couleur rouge
  rgb_arr[4 * index + 2] = scale8(b, 0xF0);  // Appliquer l'échelle à la couleur bleue
  rgb_arr[4 * index + 3] = 0;            // Couleur blanche (réservée pour SK6812)
#else  // WS2812B
  rgb_arr[3 * index] = scale8(g, 0xB0);  // Appliquer l'échelle à la couleur verte
  rgb_arr[3 * index + 1] = r;            // Appliquer la couleur rouge
  rgb_arr[3 * index + 2] = scale8(b, 0xF0);  // Appliquer l'échelle à la couleur bleue
#endif  // Fin de la différenciation des types de LEDs (SK6812 / WS2812B)
}

/**
 * @brief  Définir la couleur d'une LED spécifique (format RGBW pour SK6812)
 * @param  index: Indice de la LED (0 à NUM_PIXELS-1)
 * @param  r: Intensité du rouge (0-255)
 * @param  g: Intensité du vert (0-255)
 * @param  b: Intensité du bleu (0-255)
 * @param  w: Intensité de la couleur blanche (0-255, uniquement pour SK6812)
 * @retval None
 * @note   Cette fonction définit la couleur d'une LED dans un tableau de LEDs en utilisant les couleurs RGBW (pour SK6812).
 */
void led_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  led_set_RGB(index, r, g, b);  // Définir la couleur RGB
#if (NUM_BPP == 4)  // SK6812
  rgb_arr[4 * index + 3] = w;  // Définir la couleur blanche pour SK6812
#endif  // Fin de la différenciation des types de LEDs (SK6812 / WS2812B)
}

/**
 * @brief  Appliquer la même couleur RGB à toutes les LEDs
 * @param  r: Intensité du rouge (0-255)
 * @param  g: Intensité du vert (0-255)
 * @param  b: Intensité du bleu (0-255)
 * @retval None
 * @note   Cette fonction définit la même couleur pour toutes les LEDs du tableau.
 */
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
  for(uint_fast8_t i = 0; i < NUM_PIXELS; ++i) {
    led_set_RGB(i, r, g, b);  // Appliquer la couleur RGB à chaque LED
  }
}

/**
 * @brief  Appliquer la même couleur RGBW à toutes les LEDs (pour SK6812)
 * @param  r: Intensité du rouge (0-255)
 * @param  g: Intensité du vert (0-255)
 * @param  b: Intensité du bleu (0-255)
 * @param  w: Intensité de la couleur blanche (0-255, uniquement pour SK6812)
 * @retval None
 * @note   Cette fonction définit la même couleur pour toutes les LEDs du tableau, incluant la couleur blanche pour SK6812.
 */
void led_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for(uint_fast8_t i = 0; i < NUM_PIXELS; ++i) {
    led_set_RGBW(i, r, g, b, w);  // Appliquer la couleur RGBW à chaque LED
  }
}

/**
 * @brief  Transférer les données de couleur vers les LEDs via DMA.
 * @retval None
 * @note   Cette fonction met à jour les LEDs en envoyant les données de couleur via DMA.
 */
void led_render() {
  if(wr_buf_p != 0 || hdma_tim1_ch1.State != HAL_DMA_STATE_READY) {
    // Si un transfert est déjà en cours, annuler l'envoi et réinitialiser
    for(uint8_t i = 0; i < WR_BUF_LEN; ++i) wr_buf[i] = 0;  // Remplir le buffer avec des zéros
    wr_buf_p = 0;  // Réinitialiser la position du buffer
    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL);  // Arrêter le DMA en cours
    return;
  }

  // Remplir le buffer d'écriture avec les données des LEDs (RGB ou RGBW)
#if (NUM_BPP == 4)  // SK6812
  for(uint_fast8_t i = 0; i < 8; ++i) {
    // Remplir les bits pour chaque LED en fonction des couleurs (format RGBW pour SK6812)
    wr_buf[i     ] = PWM_LO << (((rgb_arr[0] << i) & 0x80) > 0);
    wr_buf[i +  8] = PWM_LO << (((rgb_arr[1] << i) & 0x80) > 0);
    wr_buf[i + 16] = PWM_LO << (((rgb_arr[2] << i) & 0x80) > 0);
    wr_buf[i + 24] = PWM_LO << (((rgb_arr[3] << i) & 0x80) > 0);
    wr_buf[i + 32] = PWM_LO << (((rgb_arr[4] << i) & 0x80) > 0);
    wr_buf[i + 40] = PWM_LO << (((rgb_arr[5] << i) & 0x80) > 0);
    wr_buf[i + 48] = PWM_LO << (((rgb_arr[6] << i) & 0x80) > 0);
    wr_buf[i + 56] = PWM_LO << (((rgb_arr[7] << i) & 0x80) > 0);
  }
#else  // WS2812B
  for(uint_fast8_t i = 0; i < 8; ++i) {
    // Remplir les bits pour chaque LED en fonction des couleurs (format RGB pour WS2812B)
    wr_buf[i     ] = PWM_LO << (((rgb_arr[0] << i) & 0x80) > 0);
    wr_buf[i +  8] = PWM_LO << (((rgb_arr[1] << i) & 0x80) > 0);
    wr_buf[i + 16] = PWM_LO << (((rgb_arr[2] << i) & 0x80) > 0);
    wr_buf[i + 24] = PWM_LO << (((rgb_arr[3] << i) & 0x80) > 0);
    wr_buf[i + 32] = PWM_LO << (((rgb_arr[4] << i) & 0x80) > 0);
    wr_buf[i + 40] = PWM_LO << (((rgb_arr[5] << i) & 0x80) > 0);
  }
#endif  // Fin de la différenciation SK6812 / WS2812B

  // Démarrer l'envoi des données au périphérique via DMA
  HAL_TIM_PWM_Start_DMA(&HTIM, TIM_CHANNEL, (uint32_t *)wr_buf, WR_BUF_LEN);
  wr_buf_p = 2;  // Indiquer que le buffer est prêt pour un nouvel envoi
}

/**
 * @brief  Callback de l'interruption pour la fin de la première moitié de la trame PWM (DMA).
 * @param  htim: Pointeur sur la structure du timer
 * @retval None
 * @note   Cette fonction est appelée quand la première moitié de la trame PWM a été envoyée.
 */
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  if(wr_buf_p < NUM_PIXELS) {
    // Remplir le buffer avec les données des LEDs
#if (NUM_BPP == 4)  // SK6812
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i     ] = PWM_LO << (((rgb_arr[4 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i +  8] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 16] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 2] << i) & 0x80) > 0);
      wr_buf[i + 24] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 3] << i) & 0x80) > 0);
    }
#else  // WS2812B
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i     ] = PWM_LO << (((rgb_arr[3 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i +  8] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 16] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 2] << i) & 0x80) > 0);
    }
#endif  // Fin SK6812 WS2812B
    wr_buf_p++;
  } else if (wr_buf_p < NUM_PIXELS + 2) {
    // Remplir la seconde moitié avec des zéros pour indiquer la fin de la trame
    for(uint8_t i = 0; i < WR_BUF_LEN / 2; ++i) wr_buf[i] = 0;
    wr_buf_p++;
  }
}

/**
 * @brief  Callback de l'interruption pour la fin de la seconde moitié de la trame PWM (DMA).
 * @param  htim: Pointeur sur la structure du timer
 * @retval None
 * @note   Cette fonction est appelée quand la seconde moitié de la trame PWM a été envoyée.
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  if(wr_buf_p < NUM_PIXELS) {
    // Remplir le buffer avec les données des LEDs
#if (NUM_BPP == 4)  // SK6812
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i + 32] = PWM_LO << (((rgb_arr[4 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i + 40] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 48] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 2] << i) & 0x80) > 0);
      wr_buf[i + 56] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 3] << i) & 0x80) > 0);
    }
#else  // WS2812B
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i + 24] = PWM_LO << (((rgb_arr[3 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i + 32] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 40] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 2] << i) & 0x80) > 0);
    }
#endif  // Fin SK6812 WS2812B
    wr_buf_p++;
  } else if (wr_buf_p < NUM_PIXELS + 2) {
    // Remplir la seconde moitié avec des zéros pour la réinitialisation
    for(uint8_t i = WR_BUF_LEN / 2; i < WR_BUF_LEN; ++i) wr_buf[i] = 0;
    ++wr_buf_p;
  } else {
    // Terminer le processus de transmission et réinitialiser les buffers
    wr_buf_p = 0;
    HAL_TIM_PWM_Stop_DMA(&HTIM, TIM_CHANNEL);
  }
}

/**
 * @brief  Convertir une couleur HSL (Teinte, Saturation, Luminosité) en RGB.
 * @param  h: Teinte (hue) de 0 à 255. Elle correspond à un angle sur le cercle des couleurs.
 * @param  s: Saturation de 0 à 255 (0 = gris, 255 = couleur pure).
 * @param  l: Luminosité de 0 à 255 (0 = noir, 255 = blanc).
 * @retval uint32_t: La couleur en RGB sous forme d'un entier (0xRRGGBB).
 */
uint32_t hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l) {
    if (l == 0) return 0;  // Si la luminosité est 0, la couleur est noire (absence de couleur).

    // Variables temporaires utilisées pour les calculs intermédiaires
    volatile uint8_t r, g, b, lo, c, x, m;
    volatile uint16_t h1, l1, H;

    // Ajuste la luminosité pour calculer le 'm' et 'c'
    l1 = l + 1;
    if (l < 128) {
        c = ((l1 << 1) * s) >> 8;  // Calcul de la chroma (c) dans le cas où la luminosité est faible
    } else {
        c = (512 - (l1 << 1)) * s >> 8;  // Si la luminosité est élevée, la chroma est ajustée différemment
    }

    H = h * 6;  // Convertit la teinte h de 0-255 en une valeur de 0 à 1535 (pour simplification)
    lo = H & 255;  // Calcule la valeur basse de la teinte
    h1 = lo + 1;   // Calcule l'indice de la teinte suivant

    // Calcule la valeur x, qui est utilisée pour déterminer les valeurs intermédiaires des couleurs
    if ((H & 256) == 0) {
        x = h1 * c >> 8;  // Si la teinte est dans une zone paire (par exemple, rouge à jaune)
    } else {
        x = (256 - h1) * c >> 8;  // Si la teinte est dans une zone impaire (par exemple, jaune à vert)
    }

    // Calcul du 'm' qui est l'ajustement final basé sur la luminosité
    m = l - (c >> 1);  // 'm' représente la contribution de la luminosité à la couleur

    // Calcul final des composants RGB en fonction de la teinte (H) et des valeurs intermédiaires
    switch (H >> 8) {  // La teinte est divisée en 6 segments sur le cercle des couleurs
        case 0: r = c; g = x; b = 0; break;  // Teinte entre 0° et 60° (rouge à jaune)
        case 1: r = x; g = c; b = 0; break;  // Teinte entre 60° et 120° (jaune à vert)
        case 2: r = 0; g = c; b = x; break;  // Teinte entre 120° et 180° (vert à cyan)
        case 3: r = 0; g = x; b = c; break;  // Teinte entre 180° et 240° (cyan à bleu)
        case 4: r = x; g = 0; b = c; break;  // Teinte entre 240° et 300° (bleu à magenta)
        default: r = c; g = 0; b = x; break;  // Teinte entre 300° et 360° (magenta à rouge)
    }

    // Ajoute le facteur 'm' pour ajuster la luminosité de la couleur
    return (((uint32_t)(r + m)) << 16) | (((uint32_t)(g + m)) << 8) | (uint32_t)(b + m);  // Retourne la couleur sous forme d'un entier 32 bits (0xRRGGBB)
}


/**
 * @brief  Effectuer un effet de dégradé arc-en-ciel sur les LEDs pendant une durée spécifiée
 * @param  duration_seconds: Durée du dégradé arc-en-ciel en secondes
 * @param  num_pixels: Nombre de LEDs dans la bande
 * @param  number_color_per_led: Nombre de couleurs par LED (réglage de la vitesse du dégradé)
 * @retval None
 * @note   Cette fonction fait défiler un arc-en-ciel sur les LEDs pendant la durée spécifiée.
 */
void rainbow_effect(uint16_t duration_seconds, uint8_t num_pixels, uint8_t number_color_per_led) {
    static uint8_t angle = 0;  // Initialisation de l'angle pour l'effet arc-en-ciel

    // Calcul du temps d'exécution de l'effet
    uint32_t start_time = HAL_GetTick();  // Temps actuel en millisecondes
    uint32_t end_time = start_time + (duration_seconds * 1000);  // Calcul du temps de fin de l'effet

    // Exécution de l'effet pendant la durée spécifiée
    while (HAL_GetTick() < end_time) {
        for (uint8_t i = 0; i < num_pixels; i++) {
            // Calcul de la couleur en fonction de l'angle et de la position de la LED
            uint32_t rgb_color = hsl_to_rgb(angle + (i * number_color_per_led), 255, 127);  // Teinte + décalage par LED
            // Application de la couleur calculée à la LED correspondante
            led_set_RGB(i, (rgb_color >> 16) & 0xFF, (rgb_color >> 8) & 0xFF, rgb_color & 0xFF);
        }

        // Incrément de l'angle pour faire défiler l'effet arc-en-ciel
        angle++;
        if (angle >= 360) angle = 0;  // Réinitialisation de l'angle lorsqu'il dépasse 360

        // Rendu des LEDs
        led_render();

        // Délai entre chaque changement de couleur pour contrôler la vitesse de l'animation
        HAL_Delay(10);  // Ajuster la vitesse de l'animation ici
    }

    // Une fois l'effet terminé, éteindre les LEDs
    led_set_all_RGB(0, 0, 0);  // Éteindre toutes les LEDs
    led_render();  // Appliquer la mise à jour pour éteindre les LEDs
    HAL_Delay(1);
}
