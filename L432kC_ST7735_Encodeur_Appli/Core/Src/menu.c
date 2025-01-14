#include "menu.h"  // Inclusion du fichier contenant les fonctions et variables du menu
#include <testimg_128x160.h>  // Inclusion de l'image à afficher sur l'écran

// Variables globales
static volatile uint16_t encoder_count = 0;  // Compteur de l'encodeur (stocke le nombre de tours)
static volatile int encoder_direction = 1;    // Direction de l'encodeur (+1 ou -1)
int current_option = 0;                      // Option actuelle du menu
uint8_t encoder_flag = 0;                    // Drapeau indiquant que l'encodeur a été actionné
uint8_t SW_Button_flag = 0;                  // Drapeau pour le bouton poussoir
uint8_t inSubMenu = 0;                       // Indicateur pour savoir si on est dans un sous-menu
int selectedMenuItem = 0;                    // Élément du menu actuellement sélectionné
uint32_t previousMillis = 0;                 // Horodatage précédent pour éviter les rebonds
volatile uint8_t demo_led_rgb;  			 // Drapeau déclenchement demo led rgb
volatile uint8_t motor_control;  			 // Drapeau controle moteur

// Items du menu
const char *menuItems[] = {
		"Bargraph1",        //  Affichage du bargraphe
		"Bargraph2",        //  Affichage du bargraphe
		"Afficher image",   //  Affichage de l'image
		"Eteindre LED",     //  Action pour éteindre la LED
		"Allumer LED",      //  Action pour allumer la LED
		"LED RGB",          //  Gestion des LEDs RGB (à développer)
		"MoteurCCW",            //  Gestion du moteur (à développer)
		"MoteurCW"            //  Gestion du moteur (à développer)
};

#define Item_menu (sizeof(menuItems) / sizeof(menuItems[0]))

// Fonction pour dessiner le menu principal
void drawMenu(void) {
	ST7735_FillScreenFast(COLOR_BG);  // Nettoyer l'écran avec la couleur de fond

	int visibleItems = 3;  // Nombre d'items visibles à la fois sur l'écran
	int startItem = selectedMenuItem - 1;
	//	if (startItem < 0) {
	//		startItem = 0;
	//	} else if (startItem > 3) {
	//		startItem = 3;
	//	}
	if (startItem < 0) {
		startItem = 0;
	} else if (startItem > Item_menu - visibleItems) {
		startItem = Item_menu - visibleItems;
	}

	// Boucle pour dessiner chaque item du menu
	for (int i = startItem; i < startItem + visibleItems; i++) {
		if (i < 0 || i >= Item_menu) {
			continue;
		}

		// Couleurs en fonction de la sélection de l'item
		uint16_t rectColor = (i == selectedMenuItem) ? COLOR_SELECTED : COLOR_RECT;  // Couleur du rectangle
		uint16_t textColor = (i == selectedMenuItem) ? COLOR_BG : COLOR_TEXT;        // Couleur du texte

		int rectY = RECT_Y1 + (i - startItem) * (RECT_HEIGHT + RECT_SPACING);  // Position verticale du rectangle

		// Nettoyer l'ancien dessin
		ST7735_FillRectangleFast(RECT_X, rectY, RECT_WIDTH, RECT_HEIGHT, rectColor);

		// Positionner le texte au centre du rectangle
		int textX = RECT_X + (RECT_WIDTH - strlen(menuItems[i]) * 6) / 2;  // Position X du texte
		int textY = rectY + (RECT_HEIGHT - 8) / 2;                         // Position Y du texte
		ST7735_WriteString(textX, textY, menuItems[i], Font_7x10, textColor, rectColor);  // Afficher le texte
	}
}

// Fonction pour dessiner un graphique en barre
void drawBarGraph(int value) {
	ST7735_FillScreenFast(COLOR_BG);
	// Limiter la valeur à un maximum de 100
	if (value < 0) {
		value = 0;
		encoder_count = 0;
	} else if (value > 100) {
		value = 100;
		encoder_count = 100;
	}

	// Calculer la largeur de la barre en fonction de la valeur
	int barWidth = (int)(BAR_WIDTH * (value / 100.0));

	// Définir les couleurs du bargraphe
	uint16_t greenColor = ST7735_GREEN;
	uint16_t orangeColor = ST7735_ORANGE;
	uint16_t redColor = ST7735_RED;

	// Dessiner le contour du bargraphe
	ST7735_DrawRectangle(BAR_X - 1, BAR_Y - 1, BAR_WIDTH + 2, BAR_HEIGHT + 2, ST7735_WHITE);

	// Diviser la barre en trois parties égales
	int partWidth = BAR_WIDTH / 3;

	// Remplir chaque partie avec les couleurs correspondantes
	if (barWidth > 0) {
		if (barWidth <= partWidth) {
			ST7735_FillRectangle(BAR_X, BAR_Y, barWidth, BAR_HEIGHT, greenColor);  // La partie verte
		} else if (barWidth <= 2 * partWidth) {
			ST7735_FillRectangleFast(BAR_X, BAR_Y, partWidth, BAR_HEIGHT, greenColor);   // Verte
			ST7735_FillRectangleFast(BAR_X + partWidth, BAR_Y, barWidth - partWidth, BAR_HEIGHT, orangeColor);  // Orange
		} else {
			ST7735_FillRectangleFast(BAR_X, BAR_Y, partWidth, BAR_HEIGHT, greenColor);   // Verte
			ST7735_FillRectangleFast(BAR_X + partWidth, BAR_Y, partWidth, BAR_HEIGHT, orangeColor);  // Verte + Orange
			ST7735_FillRectangleFast(BAR_X + 2 * partWidth, BAR_Y, barWidth - 2 * partWidth, BAR_HEIGHT, redColor);  // Rouge
		}
	}

	// Afficher la valeur numérique sous le graphique
	char valueText[10];
	sprintf(valueText, "%i%%", value);  // Formatage de la valeur en texte
	int textWidth = strlen(valueText) * 6;  // Largeur approximative du texte en pixels
	int textX = BAR_X + BAR_WIDTH / 2 - textWidth / 2;  // Centrer le texte horizontalement
	int textY = BAR_Y + BAR_HEIGHT + 5;  // Positionner le texte sous la barre
	ST7735_WriteString(textX, textY, valueText, Font_7x10, ST7735_WHITE, COLOR_BG);  // Affichage du texte
}

// Fonction pour dessiner le demi-cercle gradué et l'aiguille
void drawNeedle(float value) {
	ST7735_FillScreenFast(COLOR_BG);
	// Limiter la valeur entre 0 et 100
	if (value < 0) {
		value = 0;
		encoder_count = 0;
	} else if (value > 100) {
		value = 100;
		encoder_count = 100;
	}

	// 1. Dessiner le demi-cercle gradué
	for (int i = 0; i <= 10; i++) { // Affichage des graduations sur 10 points (0-100%)
		float angle = ANGLE_MIN - (ANGLE_MIN - ANGLE_MAX) * (i / 10.0); // Calculer l'angle pour chaque graduation
		int x_start = CIRCLE_X + (RADIUS - 5) * cos(angle * M_PI / 180.0);  // Position X de la graduation
		int y_start = CIRCLE_Y + (RADIUS - 5) * sin(angle * M_PI / 180.0);  // Position Y de la graduation

		int x_end = CIRCLE_X + (RADIUS) * cos(angle * M_PI / 180.0); // Position X de la fin de la graduation
		int y_end = CIRCLE_Y + (RADIUS) * sin(angle * M_PI / 180.0); // Position Y de la fin de la graduation

		// Dessiner la graduation
		ST7735_DrawLine(x_start, y_start, x_end, y_end, ST7735_WHITE);  // Remplacer par une fonction de dessin de ligne
	}

	// 2. Dessiner l'aiguille
	// L'aiguille doit être un segment de droite pivotant autour du centre du demi-cercle
	float angleNeedle = ANGLE_MIN - (ANGLE_MIN - ANGLE_MAX) * (value / 100.0); // L'angle en fonction de la valeur (0 à 100)

	int x_end_needle = CIRCLE_X + RADIUS * cos(angleNeedle * M_PI / 180.0); // Position X de l'extrémité de l'aiguille
	int y_end_needle = CIRCLE_Y + RADIUS * sin(angleNeedle * M_PI / 180.0); // Position Y de l'extrémité de l'aiguille

	// Dessiner l'aiguille
	ST7735_DrawLine(CIRCLE_X, CIRCLE_Y, x_end_needle, y_end_needle, ST7735_RED);  // Remplacer par une fonction de dessin de ligne

	// 3. Afficher le texte de la valeur
	char valueText[10];
	sprintf(valueText, "%.1f", value); // Affichage de la valeur
	int textWidth = strlen(valueText) * 6; // Largeur approximative du texte
	int textX = CIRCLE_X - textWidth / 2;
	int textY = CIRCLE_Y + RADIUS + 10; // Position du texte sous le demi-cercle
	ST7735_WriteString(textX, textY, valueText, Font_7x10, ST7735_WHITE, COLOR_BG);
}

// Fonction pour naviguer dans le menu
void navigateMenu(int direction) {
	uint32_t currentMillis = HAL_GetTick();

	// Anti-rebond pour éviter les actions trop rapprochées
	if (currentMillis - previousMillis > 100) {
		previousMillis = currentMillis;

		// Mettre à jour l'élément sélectionné
		selectedMenuItem = encoder_count % Item_menu;  // Boucle entre 0 et 7
		if (selectedMenuItem < 0) {
			selectedMenuItem += Item_menu;  // Corrige si l'encodeur a une valeur négative
		}

		drawMenu();  // Re-dessiner le menu après la navigation
	}
}

// Exécuter l'action selon l'élément du menu sélectionné
void executeMenuFunction(int menuItem) {

	switch (menuItem) {
	case 0:  // Affichage du bargraphe1
		ST7735_FillScreenFast(COLOR_BG);
		drawNeedle(25);
		break;
	case 1:  // Affichage du bargraphe2
		ST7735_FillScreenFast(COLOR_BG);
		drawBarGraph(25);  // Afficher le bargraphe avec une valeur de 25
		break;
	case 2: // Affichage de l'image
		ST7735_DrawImage(0, 0, ST7735_WIDTH, ST7735_HEIGHT, (uint16_t *)test_img_128x160);  // Afficher l'image
		break;
	case 3:  // Éteindre la LED
		ST7735_FillScreenFast(COLOR_BG);
		ST7735_WriteString((SCREEN_WIDTH-(7*11))/2, (SCREEN_HEIGHT / 2)-18/2, "LED Off", Font_11x18, ST7735_WHITE, ST7735_BLACK);
		HAL_GPIO_WritePin(LED_Green_Brd_D13_GPIO_Port, LED_Green_Brd_D13_Pin, GPIO_PIN_RESET);  // Éteindre la LED
		break;
	case 4:  // Allumer la LED
		ST7735_FillScreenFast(ST7735_GREEN);
		ST7735_WriteString((SCREEN_WIDTH-(6*11))/2, (SCREEN_HEIGHT / 2)-18/2, "LED ON", Font_11x18, ST7735_BLACK, ST7735_GREEN);
		HAL_GPIO_WritePin(LED_Green_Brd_D13_GPIO_Port, LED_Green_Brd_D13_Pin, GPIO_PIN_SET);  // Allumer la LED
		break;
		// Cas supplémentaires comme LED RGB et Moteur peuvent être ajoutés ici
	case 5:  // Gestion des LEDs RGB (à développer)
		// Fonctionnalité à développer
		ST7735_FillScreenFast(ST7735_ORANGE);
		ST7735_WriteString((SCREEN_WIDTH-(12*7))/2, (SCREEN_HEIGHT / 2)-10/2, "Demo LED RGB", Font_7x10, ST7735_BLACK, ST7735_ORANGE);
		demo_led_rgb = 1;
		break;
	case 6:  // Gestion du moteur (à développer)
		// Fonctionnalité à développer
		ST7735_FillScreenFast(ST7735_YELLOW);
		ST7735_WriteString((SCREEN_WIDTH-(10*11))/2, (SCREEN_HEIGHT / 2)-18/2, "Moteur CCW", Font_11x18, ST7735_BLACK, ST7735_YELLOW);
		motor_control = 1;
		break;
	case 7:  // Gestion du moteur (à développer)
		// Fonctionnalité à développer
		ST7735_FillScreenFast(ST7735_YELLOW);
		ST7735_WriteString((SCREEN_WIDTH-(9*11))/2, (SCREEN_HEIGHT / 2)-18/2, "Moteur CW", Font_11x18, ST7735_BLACK, ST7735_YELLOW);
		motor_control = 2;
		break;
	}
}

// Callback d'interruption pour l'encodeur rotatif
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	uint32_t currentMillis = HAL_GetTick();

	if (currentMillis - previousMillis < 10) {
		previousMillis = currentMillis;
		return;  // Ignorer si le temps est trop court
	}

	if (htim->Instance == TIM1) {
		encoder_direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(htim) ? -1 : 1;  // Détecter la direction de l'encodeur
		encoder_count += encoder_direction;  // Mettre à jour le compteur selon la direction

		if (inSubMenu) {
			// Logique spécifique au sous-menu (par exemple, pour le bargraphe)
			if (selectedMenuItem == 0) {
				drawNeedle(encoder_count); //Actualiser le bargraph type aiguille
			}
			if (selectedMenuItem == 1) {
				drawBarGraph(encoder_count); //Actualiser le bargraph type aiguille
			}
		} else {
			navigateMenu(encoder_direction);  // Si pas dans le sous-menu, naviguer dans le menu principal
		}
	}
}

// Callback d'interruption pour le bouton poussoir
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	uint32_t currentMillis = HAL_GetTick();

	if (GPIO_Pin == ENCODEUR_SWB_D0_Pin && (currentMillis - previousMillis > 100)) {
		previousMillis = currentMillis;

		if (inSubMenu) {
			inSubMenu = 0;
			drawMenu();  // Si déjà dans le sous-menu, revenir au menu principal
		} else {
			executeMenuFunction(selectedMenuItem);  // Sinon exécuter l'action du menu sélectionné
			inSubMenu = 1;  // Entrer dans le sous-menu
		}
	}
}

// Fonction pour démonstration du menu
void menu_demo(void) {
	ST7735_FillScreen(ST7735_BLACK);
	HAL_Delay(500);  // Attente d'une demi-seconde
	drawMenu();  // Afficher le menu principal
}
