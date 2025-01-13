#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "st7735.h"
#include "main.h"
#include "stdio.h"

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

// Prototypes
void drawMenu(void);
void navigateMenu(int direction);
void executeMenuFunction(int menuItem);
void menu_demo(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif /* INC_MENU_H_ */

