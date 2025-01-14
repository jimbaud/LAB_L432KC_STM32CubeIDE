#ifndef INC_ULN2803_STEPPER_H_
#define INC_ULN2803_STEPPER_H_

#include "stm32l4xx_hal.h"
#include "main.h"

#define MOTOR_COIL1_PIN_1 Stepper_In1_D11_Pin_Pin
#define MOTOR_COIL1_PIN_2 Stepper_In3_D2_Pin_Pin
#define MOTOR_COIL2_PIN_3 Stepper_In2_D12_Pin_Pin
#define MOTOR_COIL2_PIN_4 Stepper_In4_D10_Pin_Pin

#define MOTOR_COIL1_PORT_1 Stepper_In1_D11_Pin_GPIO_Port
#define MOTOR_COIL1_PORT_2 Stepper_In3_D2_Pin_GPIO_Port
#define MOTOR_COIL2_PORT_3 Stepper_In2_D12_Pin_GPIO_Port
#define MOTOR_COIL2_PORT_4 Stepper_In4_D10_Pin_GPIO_Port

typedef struct {
	GPIO_TypeDef *motorPort[4];   // Ports pour les bobines du moteur
	uint16_t motorPin[4];         // Pins pour les bobines du moteur
	uint8_t currentStep;          // Étape actuelle du moteur
	uint16_t step_delay;          // Délai entre chaque pas (vitesse)
} StepperMotor;

void Stepper_Init(StepperMotor *motor);
void Stepper_StepCW_Steps(StepperMotor *motor, uint16_t steps, uint16_t speed);
void Stepper_StepCCW_Steps(StepperMotor *motor, uint16_t steps, uint16_t speed);
void Stepper_StepCW_Half_Steps(StepperMotor *motor, uint16_t steps, uint16_t speed);
void Stepper_StepCCW_Half_Steps(StepperMotor *motor, uint16_t steps, uint16_t speed);
void Stepper_stop(StepperMotor *motor);

#endif /* INC_STEPPER_H_ */


