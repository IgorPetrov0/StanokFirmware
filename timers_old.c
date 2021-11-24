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
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);

				HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);

				HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
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
	static char step=0;
	step++;
	if(stepsCounterX>0){
		switch(step){
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
				step=0;
				break;
			}
		}
		stepsCounterX--;
		positionX+=1;
	}
	else if(stepsCounterX<0){
		switch(step){
			case(1):{
				HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_SET);
				break;
			}
			case(2):{
				HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_SET);
				break;
			}
			case(3):{
				HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_B,GPIO_PIN_SET);
				break;
			}
			case(4):{
				HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (X_A,GPIO_PIN_SET);
				step=0;
				break;
			}
		}
		stepsCounterX++;
		positionX-=1;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
void moveDirY(){
	static char step=0;
	step++;
	if(stepsCounterY>0){
		switch(step){
			case(1):{
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_SET);
				break;
			}
			case(2):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_SET);
				break;
			}
			case(3):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_SET);
				break;
			}
			case(4):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_SET);
				step=0;
				break;
			}
		}
		stepsCounterY--;
		positionY+=1;
	}
	else if(stepsCounterY<0){
		switch(step){
			case(1):{
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_SET);
				break;
			}
			case(2):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_SET);
				break;
			}
			case(3):{
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_SET);
				break;
			}
			case(4):{
				HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Y_A,GPIO_PIN_SET);
				step=0;
				break;
			}
		}
		stepsCounterY++;
		positionY-=1;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
void moveDirZ(){
	static char step=0;
	step++;
	if(stepsCounterZ>0){
		switch(step){
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
				step=0;
				break;
			}
		}
		stepsCounterZ--;
		positionZ-=1;
	}
	else if(stepsCounterZ<0){
		switch(step){
			case(1):{
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_SET);
				break;
			}
			case(2):{
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_SET);
				break;
			}
			case(3):{
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_SET);
				break;
			}
			case(4):{
				HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
				HAL_GPIO_WritePin (Z_A,GPIO_PIN_SET);
				step=0;
				break;
			}
		}
		stepsCounterZ++;
		positionZ+=1;
	}
}












