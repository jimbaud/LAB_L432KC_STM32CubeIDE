### Description du Projet

Ce projet démontre l'utilisation d'un microcontrôleur STM32 (STM32L432KC) pour gérer des périphériques variés, notamment un écran TFT, des LEDs RGB, et un moteur pas à pas. Il inclut également l'utilisation d'un encodeur rotatif pour l'interaction utilisateur.

---

### ⚠️ Note Importante

Pour que le moteur pas à pas fonctionne correctement, **il est impératif de retirer le jumper BOOT0 de la carte STM32L432KC**. Cela permet de libérer suffisamment de courant pour alimenter le moteur pas à pas.

---

### Schéma de Câblage

Le tableau suivant décrit les connexions entre les périphériques et les broches du STM32L432KC :

| **Périphérique**       | **Signal**         | **Broche STM32** | **Nom Arduino**  | **Port GPIO** | **Commentaire**                                   |
|-------------------------|--------------------|-------------|-------|---------------|--------------------------------------------------|
| **Écran TFT**          | CS                | PA4  | A3             | GPIOA         | Sélection SPI                                    |
|                        | CLK (SCL)               | PA5 | A4              | GPIOA         | Horloge SPI                                      |
|                        | MOSI (SDA)              | PA7  | A6             | GPIOA         | Données SPI                                      |
|                        | DC                | PB0    | D3           | GPIOB         | Commande/Data                                    |
|                        | RES               | PB1     | D6          | GPIOB         | Reset                                            |
| **Bande LED RGB**      | Signal            | PA6    | A5           | GPIOA         | Signal de contrôle                               |
| **Moteur Pas à Pas**   | IN1               | PB5     | D11          | GPIOB         | Commande ULN2803                                 |
|                        | IN2               | PB4      | D12         | GPIOB         | Commande ULN2803                                 |
|                        | IN3               | PA12     | D2         | GPIOA         | Commande ULN2803                                 |
|                        | IN4               | PA11      | D10        | GPIOA         | Commande ULN2803                                 |
| **Encodeur Rotatif**   | IN1 (DT)               | PA9    | D1           | GPIOA         | Signal encodeur                                  |
|                        | IN2 (CLK)              | PA8     | D9          | GPIOA         | Signal encodeur                                  |
|                        | Bouton (SW)       | PA10     | D0         | GPIOA         | Interruption externe                             |
| **LED de Debug**       | LED Verte         | PB3       | D13        | GPIOB         | Led verte carte L432KC                               |

---

### Fonctionnalités Implémentées

#### 1. **Animations de LEDs RGB**
- **Effet Arc-en-ciel** : Les LEDs changent de couleur progressivement.
- **Effet Chenille** : Une LED jaune se déplace le long de la bande.
- **Clignotements Rapides et Lents** : Les LEDs alternent entre rouge, vert, et bleu.

Exemple :
```c
led_set_all_RGB(255, 0, 0);  // Rouge
led_render();                // Appliquer les changements
HAL_Delay(500);              // Attente
```

---

#### 2. **Pilotage d’un Moteur Pas à Pas**
Le moteur **28BYJ-48** est contrôlé via un ULN2803. Deux modes sont disponibles :
- **Rotation Horaire (CW)**.
- **Rotation Anti-Horaire (CCW)**.

⚠️ **Assurez-vous que le jumper BOOT0 est retiré !**

Exemple :
```c
Stepper_StepCW_Steps(&motor, 1024, 4); // Rotation horaire sur 1024 pas
Stepper_stop(&motor);
```

---

#### 3. **Gestion d’un Écran TFT**
Un menu interactif est affiché sur un écran TFT commandé via SPI. Il permet de visualiser les actions disponibles (moteur, LEDs, etc.).

---

### Configuration Matérielle

Le projet utilise les bibliothèques suivantes :
- **STM32 HAL** : Gestion des périphériques.
- **ST7735 Driver** : Pour l’écran TFT.
- **Pixel LED Driver** : Pour la bande LED RGB.
- **ULN2803 Stepper Driver** : Pour le moteur pas à pas.

---

### Étapes pour Démarrer

1. **Préparer la Carte STM32 :**
   - Retirer le jumper BOOT0.
   - Alimenter la carte via USB ou une alimentation externe.

2. **Compiler et Flasher :**
   - Compiler le code avec STM32CubeIDE.
   - Flasher le microcontrôleur.

3. **Tester les Fonctionnalités :**
   - Les LEDs s’allument en séquence.
   - L’écran TFT affiche un menu.
   - L’encodeur rotatif permet de sélectionner et de contrôler les options.

---
