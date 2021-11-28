/*
 * drive.h
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Игорь
 */
#ifndef INC_DRIVE_H_
#define INC_DRIVE_H_



#include <math.h>
#include "stm32f1xx_hal.h"


extern TIM_HandleTypeDef htim2;
extern int counter;
extern void sendFree();


struct driversComand{
	char GCode;
	int posX;
	int posY;
	int posZ;
	int F;//подача
	int R;//радиус
	int I;//смещение центра по Х
	int J;//смещение центра по Y
	char valid;//битовое поле достовености
};

struct MComand{
	char MCode;
};


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


int positionX;
int positionY;
int positionZ;

int zeroPointX;
int zeroPointY;


int stepsCounterX;
int stepsCounterY;
int stepsCounterZ;
unsigned int timerDivider;
struct driversComand currentComand;
int lastF;

int vPosX;
int vPosY;

char relCoordinates;//>0 -относительные координаты; 0-абсолютные

void initDrives();
void executeGComand(struct driversComand comand);
void calcInterpolation();
void lineInterpolacion();
void cwCicleInterpolation();
void ccwCicleInterpolation();
void executeMComand(struct MComand comand);
void comandExecuted();
void resetCurrentCommand();


#endif /* INC_DRIVE_H_ */
