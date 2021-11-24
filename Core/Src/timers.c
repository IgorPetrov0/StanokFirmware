/*
 * timers.c
 *
 *  Created on: Aug 30, 2020
 *      Author: Игорь
 */
#include "timers.h"

////////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	static int counter=0;

	if(htim->Instance==TIM2){

		counter++;
		if(counter>=timerDivider){

			if((stepsCounterX==0) && (stepsCounterY==0) && (stepsCounterZ==0)){
				HAL_TIM_Base_Stop_IT(&htim2);
				calcInterpolation();//пересчитываем следующий шаг интерполяции
				return;
			}

			moveDirX();
			moveDirY();
			moveDirZ();
			counter=0;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
void moveDirX(){
	static char stepX=1;

	if(stepsCounterX!=0){
		if(stepsCounterX>0){
			stepX++;
			if(stepX == 5)stepX = 1;
			positionX+=1;
			stepsCounterX--;
		}
		else if(stepsCounterX<0){
			stepX--;
			if(stepX==0)stepX = 4;
			positionX-=1;
			stepsCounterX++;
		}
		switch(stepX){
			case(1):{
				HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_A,GPIO_PIN_SET);
				break;
			}
			case(2):{
				HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_B,GPIO_PIN_SET);
				break;
			}
			case(3):{
				HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_SET);
				break;
			}
			case(4):{
				HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_SET);
				break;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////
void moveDirY(){
	static char stepY=1;

	if(stepsCounterY!=0){
		if(stepsCounterY>0){
			stepY++;
			if(stepY == 5)stepY = 1;
			positionY+=1;
			stepsCounterY--;
		}
		else if(stepsCounterY<0){
			stepY--;
			if(stepY == 0)stepY = 4;
			positionY-=1;
			stepsCounterY++;
		}
		switch(stepY){
			case(1):{
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_SET);
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				break;
			}
			case(2):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_SET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				break;
			}
			case(3):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_SET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				break;
			}
			case(4):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				break;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////
void moveDirZ(){
	static char stepZ=1;

	if(stepsCounterZ!=0){
			if(stepsCounterZ>0){
				stepZ++;
				if(stepZ == 5)stepZ = 1;
				positionZ+=1;
				stepsCounterZ--;
			}
			else if(stepsCounterZ<0){
				stepZ--;
				if(stepZ == 0)stepZ = 4;
				positionZ-=1;
				stepsCounterZ++;
			}
		switch(stepZ){
			case(1):{
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_SET);
				break;
			}
			case(2):{
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_SET);
				break;
			}
			case(3):{
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_SET);
				break;
			}
			case(4):{
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_SET);
				break;
			}
		}
	}
}












