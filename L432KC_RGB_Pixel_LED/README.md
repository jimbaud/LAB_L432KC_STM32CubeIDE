# **Documentation GitHub pour la bibliothèque Pixel_LED_Driver**

## 📚 **Description**
La **Pixel_LED_Driver** est une bibliothèque logicielle optimisée pour piloter des bandes de LEDs **WS2812B** et **SK6812** à l'aide d'un **STM32**. Cette bibliothèque utilise la **PWM avec DMA** pour envoyer des signaux haute vitesse aux LEDs, ce qui minimise la charge du processeur.  

Avec cette bibliothèque, vous pouvez :  
- Allumer et éteindre des LEDs individuellement.  
- Appliquer des couleurs personnalisées à chaque LED.  
- Appliquer des couleurs globales à toutes les LEDs.  
- Réaliser des effets visuels comme l'**effet arc-en-ciel (rainbow effect)**.  

---

## ✨ **Fonctionnalités**
- **Prise en charge des LEDs WS2812B et SK6812** (via configuration `NUM_BPP`).  
- **Contrôle des couleurs RGB et RGBW** (pour SK6812).  
- **Effet arc-en-ciel paramétrable** (durée et nombre de pixels).  
- **Conversion HSL vers RGB** pour des animations fluides.  
- **Utilisation efficace des ressources** grâce au **DMA et PWM**.  

---

## 📦 **Structure des fichiers**
```
📂 /Pixel_LED_Driver
   ├── 📄 Pixel_LED_Driver.h      // Fichier d'en-tête contenant les macros et prototypes
   └── 📄 Pixel_LED_Driver.c      // Fichier source avec l'implémentation des fonctions
```

---

## ⚙️ **Configuration de la bibliothèque (Pixel_LED_Driver.h)**
La configuration s'effectue principalement dans le fichier **`Pixel_LED_Driver.h`**. Vous y trouverez plusieurs paramètres importants.

### **1️⃣ Type de LED**
Choisissez le type de LED utilisé (WS2812B ou SK6812) :  
```c
#define LED_TYPE           LED_TYPE_WS2812B  // Ou LED_TYPE_SK6812
```
- **LED_TYPE_WS2812B** — 3 canaux de couleurs (R, G, B).  
- **LED_TYPE_SK6812** — 4 canaux de couleurs (R, G, B, W).  

---

### **2️⃣ Nombre de LEDs**
Configurez le nombre total de LEDs dans la bande.  
```c
#define NUM_PIXELS         (12) // Nombre total de LEDs
```
Changez la valeur de **`NUM_PIXELS`** selon le nombre de LEDs dans votre bande LED.

---

### **3️⃣ Paramètres des signaux PWM**
Le contrôle des LEDs nécessite des signaux spécifiques (PWM) pour les bits logiques **0** et **1**.  
```c
#define PWM_HI             (38)  // Durée du signal "1" logique
#define PWM_LO             (19)  // Durée du signal "0" logique
```
Ces valeurs dépendent de la fréquence de la **PWM** et de la bande de LED utilisée.  

---

### **4️⃣ Paramètres de la transmission DMA**
- **HTIM** : Nom du **Timer** utilisé.  
- **DMA_CHANNEL** : Nom du **DMA** associé au canal PWM.  
- **TIM_CHANNEL** : Canal utilisé sur le Timer (exemple : TIM_CHANNEL_1).  
Ces paramètres permettent d'adapter la bibliothèque à n'importe quel timer et canal DMA de votre STM32.  
```c
#define HTIM                htim1
#define DMA_CHANNEL         hdma_tim1_ch1
#define TIM_CHANNEL         TIM_CHANNEL_1
```
Si vous souhaitez passer au **Timer 2**, remplacez simplement `htim1` par `htim2` et **aucun autre changement dans la bibliothèque ne sera nécessaire**.
(pensez aussi à ajuster le cannal DMA et le cannal PWM !)

---

## ⚙️ **Configuration du Timer et du DMA**
Pour que la bibliothèque fonctionne, il est nécessaire de configurer correctement le Timer et le DMA comme illustré :  

<p align="center">
  <img src="./img/Config_pwm.png" alt="Configuration PWM" width="70%">
</p>

<p align="center">
  <img src="./img/Config_dma.png" alt="Configuration DMA" width="70%">
</p>

---

## 🚀 **Utilisation des fonctions**
### **1️⃣ led_set_RGB**
> Définit la couleur d'une LED via ses composantes **Rouge, Vert, Bleu** (RVB).  
```c
led_set_RGB(0, 255, 0, 0); // Allume la première LED en rouge
```

### **2️⃣ led_set_all_RGB**
> Applique la même couleur **RVB** à toutes les LEDs.  
```c
led_set_all_RGB(0, 255, 0); // Toutes les LEDs sont vertes
```

### **3️⃣ led_render**
> Met à jour toutes les LEDs avec les couleurs actuelles.  
```c
led_render();
```

### **4️⃣ rainbow_effect**
> Affiche un **effet arc-en-ciel** dynamique sur la bande de LEDs.  
```c
rainbow_effect(10, 12, 15); // Effet arc-en-ciel pendant 10s sur 12 LEDs
```

---

## 🎨 **Effet Arc-en-Ciel (Rainbow)**
L'effet **rainbow_effect()** permet d'afficher un dégradé de couleurs sur les LEDs.  
### Paramètres :
| **Paramètre**       | **Description**              |
|---------------------|-----------------------------|
| `duration_seconds`  | Durée totale de l'effet en secondes |
| `num_pixels`        | Nombre de LEDs sur la bande  |
| `number_color_per_led` | Décalage des couleurs sur chaque LED |

**Exemple** :  
```c
rainbow_effect(10, 12, 10); // 10 secondes d'effet rainbow sur 12 LEDs avec 10 couleurs 
```

---

## 🔧 **Exemple d'utilisation complet**
```c
#include "Pixel_LED_Driver.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_TIM1_Init();
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    // Initialisation des LEDs
    led_set_all_RGB(0, 0, 0); // Éteint toutes les LEDs
    led_render();

    // Affiche l'effet arc-en-ciel
    rainbow_effect(10, 12, 15);

    while (1) {
        // Boucle principale
    }
}
```

---

## ⚠️ **Dépannage**
| **Problème**         | **Cause possible**            | **Solution**                   |
|---------------------|---------------------------------|----------------------------------|
| Les LEDs ne s'allument pas | Mauvaise configuration du timer | Vérifiez le prescaler, period et PWM |
| Les couleurs ne sont pas correctes | Mauvaise configuration des bits PWM | Vérifiez **PWM_HI** et **PWM_LO** |
| La première LED ne réagit pas | Buffer mal aligné | Augmentez la longueur du buffer de **+1** |
| L'effet arc-en-ciel ne défile pas | Mauvais calcul de HSL | Vérifiez le décalage de la teinte |

---

## **Rendu**

<p align="center">
  <img src="./img/led1.jpg" alt="Configuration PWM" width="70%">
</p>

<p align="center">
  <img src="./img/led2.jpg" alt="Configuration DMA" width="70%">
</p>

Voici la **section "Schéma de connexion"** à intégrer dans le fichier GitHub, ainsi que des **références sur les LEDs adressables**.

---

## 📐 **Schéma de connexion des LEDs avec la Nucleo L432KC**

Voici le schéma de câblage ASCII pour relier une bande de LEDs adressables **WS2812B** ou **SK6812** à la carte **Nucleo-32 L432KC**.

```plaintext
         ----------------------------
        |        NUCLEO-32           |
        |                            |
        |     3.3V  [ ] (1)           |
        |      5V   [ ] (2)  -->  +5V (Alimentation des LEDs)
        |      GND  [ ] (3)  -->  GND (Alimentation des LEDs)
        |     D9   [ ] (4)  -->  DIN (Entrée de la LED)
        |                            |
         ----------------------------

                           ┌─────────────┐
      +5V  ----------------┤  VDD (5V)   │
      GND  ----------------┤  GND        │
      D9   ----------------┤  DIN        │
                           └─────────────┘
                                LED 1
                           ┌─────────────┐
                           │  VDD (5V)   │
                           │  GND        │
          DOUT ------------┤  DIN        │
                           └─────────────┘
                                LED 2
                           ┌─────────────┐
                           │  VDD (5V)   │
                           │  GND        │
          DOUT ------------┤  DIN        │
                           └─────────────┘
                                ...
```

---

### 🔍 **Explications des connexions**
| **Nom**         | **Description**                          |
|-----------------|------------------------------------------|
| **5V**          | Alimentation des LEDs (broche Nucleo 5V) |
| **GND**         | Masse commune (broche Nucleo GND)        |
| **D9**          | Signal de données (broche Nucleo D9)     |
| **VDD**         | Alimentation des LEDs (5V)               |
| **DIN**         | Entrée de la LED (signal de commande)    |
| **DOUT**        | Sortie de la LED (vers la LED suivante)  |

---

### 🛠️ **Détails techniques**
1. **Alimentation des LEDs** :
   - La bande de LEDs nécessite une alimentation en **5V** et **GND**.
   - La carte **Nucleo-32 L432KC** peut fournir cette tension via sa broche **5V**.  
   ⚠️ **Attention :** Assurez-vous que l'intensité maximale demandée par la bande de LEDs ne dépasse pas la capacité de la carte Nucleo.

2. **Signal de contrôle (DIN)** :
   - Le signal de données provient de la broche **D9** de la carte Nucleo, qui envoie des impulsions via le **PWM**.
   - La **PWM est configurée via le Timer TIM1** et associée à la broche **PA8 (D9)**.

3. **Transmission de signal** :
   - Le signal passe de **DOUT** de la LED 1 à **DIN** de la LED 2, créant une chaîne de LEDs.
   - Chaque LED retransmet le signal à la LED suivante.

---

## 🌐 **Références sur les LEDs adressables**
- **WS2812B** : [Fiche technique WS2812B](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)  
- **SK6812** : [Fiche technique SK6812](https://cdn-shop.adafruit.com/product-files/1138/SK6812.pdf)  
- **Tutoriel sur les LEDs WS2812B** : [Guide WS2812B sur Adafruit](https://learn.adafruit.com/adafruit-neopixel-uberguide)  
- **Explication du signal de contrôle** : [Page du signal de contrôle WS2812B](https://wp.josh.com/2014/05/13/ws2812-neopixels-are-cheaper-than-you-think/)  

Ces références fournissent des explications techniques sur la manière dont les LEDs reçoivent et relaient les signaux, ainsi que sur les paramètres de timing des impulsions **PWM** nécessaires au bon fonctionnement des **WS2812B** et **SK6812**.

---
