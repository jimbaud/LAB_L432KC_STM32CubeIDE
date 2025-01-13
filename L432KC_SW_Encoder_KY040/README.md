---

# **Projet : Gestion d'un encodeur rotatif avec STM32**

## **Description**
Ce projet utilise un microcontrôleur STM32 pour lire les données d'un encodeur rotatif et gérer un bouton poussoir intégré. Les données sont transmises via UART pour affichage dans un terminal.

---

## **Câblage**

### **Broches utilisées :**
| Nom de la broche STM32 | Fonction             | Broche de l'encodeur rotatif |
|-------------------------|----------------------|--------------------------------|
| `PA8`                  | Signal A (CH1)      | DT                             |
| `PA9`                  | Signal B (CH2)      | CLK                            |
| `PA10`                 | Bouton poussoir     | SW                             |

### **Schéma du câblage :**
1. Connectez la broche **DT** de l'encodeur rotatif à **PA8**.
2. Connectez la broche **CLK** de l'encodeur rotatif à **PA9**.
3. Connectez la broche **SW** (bouton poussoir) de l'encodeur rotatif à **PA10**.
4. Alimentez l'encodeur rotatif avec le 3.3V et GND du STM32.

### **Remarques :**
- Utilisez des résistances pull-up ou activez-les dans le microcontrôleur si nécessaire.
- Assurez-vous de bien connecter la masse de l'encodeur avec celle du STM32.

---

## **Fonctionnalités principales**
1. **Lecture des impulsions de l'encodeur rotatif** :
   - Le timer `TIM1` est configuré en mode encodeur pour détecter les impulsions.
   - Le sens de rotation est déterminé via l'état du compteur (`counting up` ou `counting down`).

2. **Gestion des appuis sur le bouton poussoir** :
   - Une interruption GPIO est configurée sur **PA10**.
   - Les rebonds sont filtrés par un délai logiciel de 50 ms.

3. **Affichage UART** :
   - Les données de l'encodeur (valeur et direction) ainsi que l'état du bouton poussoir sont envoyés via UART pour être affichés dans un terminal série.

---

## **Fichiers du projet**
- **`main.c`** : Point d'entrée principal, boucle principale.
- **`Rot_Encoder.h`** : Header pour les fonctions et variables de l'encodeur.
- **`tim.c` et `tim.h`** : Configuration du timer pour le mode encodeur.
- **`usart.c` et `usart.h`** : Configuration de l'UART.
- **`gpio.c` et `gpio.h`** : Configuration des GPIO pour l'encodeur et le bouton.

---

## **Exécution**

### **1. Préparation**
- Connectez le STM32 à votre PC via un convertisseur USB-UART.
- Utilisez un logiciel de terminal série (par exemple : PuTTY ou Tera Term) :
  - **Baud rate** : 115200
  - **Data bits** : 8
  - **Stop bits** : 1
  - **Parity** : None

### **2. Compilation et flash**
- Compilez le projet avec STM32CubeIDE ou un autre IDE compatible.
- Flashez le firmware sur le microcontrôleur.

### **3. Résultat attendu**
- Faites tourner l'encodeur dans un sens ou l'autre. La valeur incrémentée ou décrémentée est affichée dans le terminal.
- Appuyez sur le bouton poussoir pour afficher un message d'appui.

---
