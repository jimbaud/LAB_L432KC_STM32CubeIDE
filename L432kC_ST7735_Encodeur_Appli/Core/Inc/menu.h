#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "st7735.h"
#include "main.h"
#include "stdio.h"
#include "Pixel_LED_Driver.h"
#include "ULN2803_Stepper.h"
#include "math.h"

// Dimensions de l'écran
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 160

// Couleurs
#define COLOR_BG        ST7735_BLACK
#define COLOR_RECT      ST7735_BLUE
#define COLOR_TEXT      ST7735_WHITE
#define COLOR_SELECTED  ST7735_YELLOW


// Dimensions des rectangles du menu
#define RECT_WIDTH       120
#define RECT_HEIGHT      40
#define RECT_SPACING     10
#define RECT_X           (SCREEN_WIDTH / 2 - RECT_WIDTH / 2)
#define RECT_Y1          10

// Dimensions du bargraph
#define BAR_WIDTH        100
#define BAR_HEIGHT       20
#define BAR_X            (SCREEN_WIDTH / 2 - BAR_WIDTH / 2)
#define BAR_Y            (SCREEN_HEIGHT / 2 - BAR_HEIGHT / 2)

// Définir les constantes pour le demi-cercle avec aiguille
#define CIRCLE_X 64   // Coordonnée X du centre du demi-cercle
#define CIRCLE_Y 80   // Coordonnée Y du centre du demi-cercle
#define RADIUS 50     // Rayon du demi-cercle
#define ANGLE_MIN 180 // Angle de départ (0° - 180°)
#define ANGLE_MAX 0   // Angle d'arrivée (180° - 0°)

//Variables partagées
extern volatile uint8_t demo_led_rgb;
extern volatile uint8_t motor_control;
extern StepperMotor motor;

// Prototypes
void drawMenu(void);
void navigateMenu(int direction);
void executeMenuFunction(int menuItem);
void menu_demo(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif /* INC_MENU_H_ */

