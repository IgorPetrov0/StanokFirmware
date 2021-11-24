/*
 * drive.c
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Игорь
 */


#include "drive.h"


void initDrives(){
	positionX=0;
	positionY=0;
	positionZ=0;

	stepsCounterX=0;
	stepsCounterY=0;
	stepsCounterZ=0;
	timerDivider=0;

	zeroPointX=0;
	zeroPointY=0;

	lastF=0;

}
//////////////////////////////////////////////////////////////////////////////////////
void executeGComand(struct driversComand comand){

	currentComand = comand;

	if(comand.valid & (1<<3)){//если подача валидна,
		lastF = comand.F;//то запоминаем её. В следующей команде подачи может не быть
	}
	//одно прерывание таймера = 0.000 025 С
	float F=(float)lastF/100;
	float periodPerStep=1/(1/F/200);
	timerDivider=40000/periodPerStep;

	switch(comand.GCode){
		case(0):{//холостое перемещение

			if(comand.valid & (1<<1)){
				stepsCounterX = comand.posX - positionX;
			}

			if(comand.valid & (1<<2)){
				stepsCounterY = comand.posY - positionY;
			}

			if(comand.valid & (1<<3)){
				stepsCounterZ = comand.posZ - positionZ;
			}
			HAL_TIM_Base_Start_IT(&htim2);
			break;
		}
		case(1):{//линейное перемещение
			if(comand.valid & (1<<2)){//если Z валидна,
				int difZ=positionZ-comand.posZ;//то задаем для нее количество шагов
				stepsCounterZ=difZ*10;
			}
			if((comand.valid & (1<<0)) || (comand.valid & (1<<1))){//если х или у валидны
					zeroPointX = positionX;
					zeroPointY = positionY;
					calcInterpolation();//считаем интерполяцию
			}
			break;
		}
		case(2):{//круговая интерполяция по часовой стрелке
			//есть два варианта команды:через радиус или через координаты центра
			//в первом случае координатами центра считаем центр хорды

			if(comand.valid & (1<<4)){//если строим через радиус
				//находим середину хорды
				int dX = positionX - comand.posX;
				int dY = positionY - comand.posY;

			}
			else{//если через координаты центра
				if(comand.valid & (1<<5)){//если I валидна

				}
				if(comand.valid & (1<<6)){//если J валидна

				}
			}






			//находим половину длины хорды
			int d = sqrt((dX * dX) + (dY * dY));
			//находим длину перпендикуляра к хорде
			int n = sqrt((comand.R * comand.R) + (d * d));

			calcInterpolation();
			break;
		}
		case(3):{//круговая интерполяция против часовой стрелки
			//находим длину хорды
			int a = positionX - comand.posX;
			int b = positionY - comand.posY;
			int d = abs(sqrt((a * a) + (b * b)));
			vPosX=0;
			vPosY=d/2;//половина хорды
			calcInterpolation();
			break;
		}
		case(92):{//установка в нулевую позицию
			positionX=0;
			positionY=0;
			positionZ=0;
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////
void calcInterpolation(){
	int tmpX=positionX;
	int tmpY=positionY;
	if(!(currentComand.valid & (1<<0))){//если X не валидна
		tmpX = currentComand.posX;//то не сравниваем
	}
	if(!(currentComand.valid & (1<<1))){//если Y не валидна
		tmpY = currentComand.posY;//то не сравниваем
	}

	if((tmpX == currentComand.posX) && (tmpY == currentComand.posY)){//если пришли в заданную позицию, то ничего не делаем
		vPosX=0;
		vPosY=0;
		return;
	}

	switch(currentComand.GCode){
		case(1):{//линейное перемещение
			lineInterpolacion();
			break;
		}
		case(2):{//круговая интерполяция по часовой стрелке
			cwCicleInterpolation();
			break;
		}
		case(3):{//круговая интерполяция против часосвой стрелки
			ccwCicleInterpolation();
			break;
		}
		default:{
			return;//todo потом сделать обработку ошибки
		}
	}
	HAL_TIM_Base_Start_IT(&htim2);//запускаем таймер
}
////////////////////////////////////////////////////////////////////////////////////
void executeMComand(struct MComand comand){
	switch(comand.MCode){
		case(18):{
			HAL_GPIO_WritePin (X_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (X_B,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (X_D,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (X_C,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Y_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Y_B,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Y_D,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Y_C,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Z_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Z_B,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Z_D,GPIO_PIN_RESET);
			HAL_GPIO_WritePin (Z_C,GPIO_PIN_RESET);
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////
void lineInterpolacion(){
	int offsetX = zeroPointX - positionX;
	int offsetY = zeroPointY - positionY;
	int vectorX = currentComand.posX - positionX;
	int vectorY = currentComand.posY - positionY;

	int F = (abs(offsetY) * abs(vectorX) - (abs(offsetX) * abs(vectorY)));//оценочная функция
	if(F == 0){
		if(vectorX > 0){
			stepsCounterX = 1;
		}
		else if(vectorX < 0){
			stepsCounterX = -1;
		}
		else{
			stepsCounterX = 0;
		}
		if(vectorY > 0){
			stepsCounterY = 1;
		}
		else if(vectorY < 0){
			stepsCounterY = -1;
		}
		else{
			stepsCounterY = 0;
		}
	}
	else if(F > 0){
		if(vectorX > 0){
			stepsCounterX = 1;
		}
		else if(vectorX < 0){
			stepsCounterX = -1;
		}
		else{
			stepsCounterX = 0;
			if(vectorY > 0){
				stepsCounterY = 1;
			}
			else if(vectorY < 0){
				stepsCounterY = -1;
			}
			else{
				stepsCounterY = 0;
			}
		}
	}
	else if(F < 0){
		if(vectorY > 0){
			stepsCounterY = 1;
		}
		else if(vectorY < 0){
			stepsCounterY = -1;
		}
		else{
			stepsCounterY = 0;
			if(vectorX > 0){
				stepsCounterX = 1;
			}
			else if(vectorX < 0){
				stepsCounterX = -1;
			}
			else{
				stepsCounterX = 0;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void cwCicleInterpolation(){

	int F = ((vPosX * vPosX) + (vPosY * vPosY)) - (currentComand.R * currentComand.R);//оценочная функция (радиус всегда положительный)
																		//контролируется на этапе парсинга
	if(F >= 0){//если на окружности или за ней

		if((vPosX > 0) && (vPosY > 0)){//1 квадрант
			stepsCounterY = 1;
		}
		else if((vPosX > 0) && (vPosY < 0)){//2 квадрант
			stepsCounterX = -1;
		}
		else if((vPosX < 0) && (vPosY < 0)){//3 квадрант
			stepsCounterY = -1;
		}
		else if((vPosX < 0) && (vPosY > 0)){//4 квадрант
			stepsCounterX = 1;
		}
		else{
			if(vPosY > 0){
				stepsCounterY = 1;//должен быть -, но инвертировано для привода
			}
			else if(vPosY < 0){
				stepsCounterY = -1;
			}
			else if(vPosX > 0){
				stepsCounterX = -1;
			}
			else if(vPosX < 0){
				stepsCounterX = 1;
			}
		}
	}
	else if(F < 0){//если внутри окружности
		if((vPosX > 0) && (vPosY > 0)){//1 квадрант
			stepsCounterX = 1;
		}
		else if((vPosX > 0) && (vPosY < 0)){//2 квадрант
			stepsCounterY = 1;
		}
		else if((vPosX < 0) && (vPosY < 0)){//3 квадрант
			stepsCounterX = -1;
		}
		else if((vPosX < 0) && (vPosY > 0)){//4 квадрант
			stepsCounterY = -1;
		}
		else{
			if(vPosY > 0){
				stepsCounterX = 1;
			}
			else if(vPosY < 0){
				stepsCounterX = -1;
			}
			else if(vPosX > 0){
				stepsCounterY = 1;
			}
			else if(vPosX < 0){
				stepsCounterY = -1;
			}
		}
	}

	vPosX+=stepsCounterX;
	vPosY-=stepsCounterY;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void ccwCicleInterpolation(){
	int offsetX = zeroPointX - positionX + currentComand.R;//доворачиваеем систему координат, чтобы начальная точка лежала на оси X
	int offsetY = zeroPointY - positionY;

	int absOffsetX = abs(offsetX);
	int absOffsetY = abs(offsetY);
	int F = ((absOffsetX*absOffsetX) + (absOffsetY*absOffsetY)) - (currentComand.R * currentComand.R);//оценочная функция (радиус всегда положительный)
																									//контролируется на этапе парсинга

	if((offsetX > 0) && (offsetY > 0)){//1 квадрант
		if(F >= 0){
			stepsCounterX = -1;//шаг влево
		}
		else{
			stepsCounterY = 1;//шаг вверх
		}
	}
	else if((offsetX > 0) && (offsetY < 0)){//2 квадрант
		if(F >= 0){
			stepsCounterX = 1;//шаг вправо
		}
		else{
			stepsCounterY = 1;//шаг вверх
		}
	}
	else if((offsetX < 0) && (offsetY > 0)){//3 квадрант
		if(F >= 0){
			stepsCounterX = 1;//шаг вправо
		}
		else{
			stepsCounterY = 1;//шаг вниз
		}
	}
	else if((offsetX < 0) && (offsetY > 0)){//4 квадрант
		if(F >= 0){
			stepsCounterX = -1;//шаг влево
		}
		else{
			stepsCounterY = -1;//шаг вниз
		}
	}
}
