/*
 * timers.h
 *
 *  Created on: Aug 30, 2020
 *      Author: Игорь
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include "stm32f1xx_hal.h"


#define Y_A GPIOB, GPIO_PIN_6
#define Y_B GPIOA, GPIO_PIN_15
#define Y_C GPIOB, GPIO_PIN_3
#define Y_D GPIOB, GPIO_PIN_4

#define X_A GPIOB, GPIO_PIN_12
#define X_B GPIOB, GPIO_PIN_13
#define X_C GPIOB, GPIO_PIN_14
#define X_D GPIOB, GPIO_PIN_15

#define Z_A GPIOA, GPIO_PIN_8
#define Z_B GPIOA, GPIO_PIN_9
#define Z_C GPIOA, GPIO_PIN_10
#define Z_D GPIOB, GPIO_PIN_7

extern int positionX;
extern int positionY;
extern int positionZ;

extern int stepsCounterX;
extern int stepsCounterY;
extern int stepsCounterZ;
extern unsigned int timerDivider;
extern TIM_HandleTypeDef htim2;
extern void calcInterpolation();

void moveDirX();
void moveDirY();
void moveDirZ();


#endif /* SRC_TIMERS_H_ */
