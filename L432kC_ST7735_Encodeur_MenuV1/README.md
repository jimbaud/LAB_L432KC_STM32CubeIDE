
---

### **Projet : Menu Système avec SPI et Contrôle d'Écran**

Ce projet présente un système de menu interactif développé sur une plateforme STM32L4 utilisant le périphérique SPI pour l'affichage graphique via un écran tactile. Le système met en œuvre des fonctions de navigation, des bargraphs et des interactions avec des capteurs rotatifs et des boutons poussoirs.

---

### **Caractéristiques principales du projet :**
- **SPI Configuration** : Gestion des paramètres de l’interface SPI (Mode maître, Polarity, Phase, etc.) pour la communication avec l'écran.
- **Menu Graphique** : Affichage dynamique des menus sur l’écran TFT utilisant des rectangles, des textes et des couleurs.
- **Navigation** : Permet à l'utilisateur de naviguer parmi les différentes options du menu via un encodeur rotatif.
- **Interactivité** : Réalise des actions spécifiques selon les choix de l'utilisateur et les signaux externes comme les boutons poussoirs.
- **Bargraph** : Affichage graphique de données sous forme de bargraphe, coloré selon les valeurs.
- **Gestion des interruptions** : Gère les interruptions des périphériques tels que l'encodeur rotatif et le bouton poussoir.

---

### **Schema de connexion** :

Voici un diagramme détaillé de la connexion des composants principaux :

```
                                +-----------------+
                                |         STM32L4   |
                                |                 o
                                |                 o
                                |                 o
                                |  +-----------------+
                                |  | TFT SPI Display  |
                                |  +-----------------+
                                |   |   SPI1_SCK (PA5)  |
                                |   |   SPI1_MOSI (PA7) |
                                |   |   TFT_CS (PA4)    |
                                |   |   TFT_DC (PB0)    |
                                |   |   TFT_RESET (PB1) |
                                |   +-----------------+
                                |   |   Encoder IN1 (PA9) |
                                |   |   Encoder IN2 (PA8) |
                                |   |   Encoder SWB (PA10) |
                                |   |   LED (PB3)         |
                                |   +-----------------+
+-----------------------------+
|                             |
|    +-------------------+   |
|    |   SPI1 Interface   |   |
|    +-------------------+   |
|    |     +--------------+  |
|    |     | GPIO Ports    |  |
|    |     | (PA5, PA7, PB0)|  |
|    |     +--------------+  |
|    |                             |
|    |                             |
+-----------------------------+
```

| **Périphérique**                | **Broche MCU**      | **Description**                   |
|----------------------------------|----------------------|-----------------------------------|
| **Écran TFT**                     |                      | Écran graphique TFT                |
| TFT_SPI_CLK                       | PA5                  | Horloge SPI                         |
| TFT_SPI_MOSI (SDA)                | PA7                  | Données SPI                         |
| TFT_DC                            | PB0                  | Signal de commande/données         |
| TFT_RESET                         | PB1                  | Signal de reset                    |
| TFT_PIN_CS                        | PA4                  | Chip Select (CS) pour TFT          |
| **Encodeur rotatif**              |                      | Encodeur rotatif                   |
| ENCODER_IN1 (DT)                  | PA9                  | Signal A de l'encodeur              |
| ENCODER_IN2 (CLK)                 | PA8                  | Signal B de l'encodeur              |
| ENCODEUR_SW (SW)                  | PA10                 | Bouton poussoir de l'encodeur      |
| **LED**                           |                      | LEDs de signalisation               |
| LED_GREEN                         | PB3                  | LED verte de la carte Nucleo       |

---

### **Exemples de fonctionnalités** :
- **Bargraph** : Affiche les données sous forme de bargraphes colorés, avec indication de pourcentage.
- **Menu** : Permet à l'utilisateur de naviguer dans différentes options de menus, sélectionnables via un encodeur rotatif et un bouton poussoir.
- **Interrupteurs** : Réalise les actions définies par les boutons poussoirs (action sur la LED ou d’autres options).
- **Affichage** : Affiche une image sur l'écran TFT à l'aide du module SPI.

---

### **Environnement de développement** :
- Développement basé sur le microcontrôleur STM32L4.
- Utilisation des bibliothèques HAL (STM32Cube HAL) pour la programmation des périphériques.
- Interface SPI pour la communication avec l’écran TFT.
- Gestion des interruptions pour les entrées et sorties.

---

### **Installation et utilisation** :
- Connectez le microcontrôleur STM32L4 selon le schéma de connexion illustré.
- Assurez-vous que les bibliothèques SPI et GPIO sont configurées correctement.
- Téléchargez le firmware via le STM32CubeProgrammer ou un autre outil de flashage compatible.
- Compilez et programmez le code en utilisant un IDE compatible STM32 (par exemple, STM32CubeIDE).
- Une fois le programme chargé, vous pouvez naviguer à l’aide de l’encodeur rotatif et des boutons poussoirs, et observer les actions sur l'écran TFT.

---

## **Script de Conversion d'Images pour Écran TFT**

### **Introduction**
Ce projet vous permet de transformer une image en une série de données utilisables sur un écran TFT, en particulier les écrans utilisant la bibliothèque graphique avec le microcontrôleur STM32. Le script Python prend en charge l’importation de l’image, la conversion et l’exportation des pixels sous forme de tableau de données, prêt à être intégré dans des projets de développement électronique. Le script est disponible dans le sous répertoire **Tools**.

### **Caractéristiques du projet**
- **Script de conversion** : Le script convertit des images en fichiers C sous forme de tableaux de pixels hexadécimaux.
- **Soutien aux écrans TFT** : Idéal pour les écrans avec une résolution de 128x160 pixels.

### **Script Python** : 
```python
#!/usr/bin/env python

# vim: set ai et ts=4 sw=4:

from PIL import Image
import sys
import os

if len(sys.argv) < 2:
    print("Usage: {} <image-file>".format(sys.argv[0]))
    sys.exit(1)

fname = sys.argv[1]

img = Image.open(fname)
if img.width > 128 or img.height > 160:
    print("Error: max 128x80 image expected")
    sys.exit(2)

# Créez une nouvelle image blanche de 128x160 pixels
new_img = Image.new("RGB", (128, 160), (255, 255, 255))

# Collez l'image d'origine au centre de la nouvelle image
x_offset = (128 - img.width) // 2
y_offset = (160 - img.height) // 2
new_img.paste(img, (x_offset, y_offset))

print("const uint16_t test_img_160x80[][128] = {")
new_img = new_img.convert("RGB")

for y in range(0, new_img.height):
    s = "{"
    for x in range(0, new_img.width):
        (r, g, b) = new_img.getpixel((x, y))
        color565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)
        # Pour s'assurer que cela fonctionne avec le bon endianness pour les écrans ST7735
        color565 = ((color565 & 0xFF00) >> 8) | ((color565 & 0xFF) << 8)
        s += "0x{:04X},".format(color565)
    s += "},"
    print(s)

print("};")
```

### **Instructions pour l’utilisation du script**
1. **Téléchargez l’image** : Placez l’image que vous souhaitez convertir dans le même dossier que le script.
2. **Exécutez le script** : En ligne de commande, exécutez le script en passant le chemin de l’image en paramètre :
   ```bash
   python convert_image.py <chemin_vers_votre_image.jpg>
   ```
3. **Résultat** : Le script génère un tableau C qui contient les pixels sous forme de données hexadécimales, adaptées pour l’affichage sur des écrans TFT de 128x160 pixels.

---
