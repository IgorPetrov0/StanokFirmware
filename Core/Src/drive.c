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

	relCoordinates=0;//по умолчанию координаты абсолютные

}
//////////////////////////////////////////////////////////////////////////////////////
void executeGComand(struct driversComand comand){

	currentComand = comand;

	if(currentComand.valid & (1<<3)){//если подача валидна,
		lastF = currentComand.F;//то запоминаем её. В следующей команде подачи может не быть
	}

	//если координаты абсолютные заполняем невалидные параметры текущими значениями
	if(relCoordinates == 0){
		if(!(currentComand.valid & (1<<0))){
			currentComand.posX = positionX;
		}
		if(!(currentComand.valid & (1<<1))){
			currentComand.posY = positionY;
		}
		if(!(currentComand.valid & (1<<2))){
			currentComand.posZ = positionZ;
		}
		if(currentComand.valid & (1<<5)){//если I валидна, то преобразуем ее в абсолютные координаты
			currentComand.I = positionX + currentComand.I;
		}
		else{
			currentComand.I = positionX;//иначе просто присваиваем позицию Х
		}
		if(currentComand.valid & (1<<6)){//если J валидна, то преобразуем ее в абсолютные координаты
			currentComand.J = positionY + currentComand.J;
		}
		else{
			currentComand.J = positionY;//иначе просто присваиваем позицию Y
		}
	}

	//одно прерывание таймера = 0.000 025 С
	float F=(float)lastF/100;
	float periodPerStep=1/(1/F/200);
	timerDivider=40000/periodPerStep;

	switch(currentComand.GCode){
		case(0):{//холостое перемещение
			if(relCoordinates > 0){//если координаты относительные - пересчитываем в абсолютные
				currentComand.posX = currentComand.posX + positionX;
				currentComand.posY = currentComand.posY + positionY;
				currentComand.posZ = currentComand.posZ + positionZ;
			}
			stepsCounterX = currentComand.posX - positionX;
			stepsCounterY = currentComand.posY - positionY;
			stepsCounterZ = currentComand.posZ - positionZ;

			HAL_TIM_Base_Start_IT(&htim2);
			break;
		}
		case(1):{//линейное перемещение
			if(relCoordinates > 0){//если координаты относительные - пересчитываем в абсолютные
				currentComand.posX = currentComand.posX + positionX;
				currentComand.posY = currentComand.posY + positionY;
				currentComand.posZ = currentComand.posZ + positionZ;
			}

			if(currentComand.valid & (1<<2)){//если Z валидна,
				int difZ=positionZ-currentComand.posZ;//то задаем для нее количество шагов
				stepsCounterZ=difZ*10;
			}
			if((currentComand.valid & (1<<0)) || (currentComand.valid & (1<<1))){//если х или у валидны
					zeroPointX = positionX;
					zeroPointY = positionY;
			}
			calcInterpolation();//считаем интерполяцию
			break;
		}
		case(2):{//круговая интерполяция по часовой стрелке
			if(relCoordinates > 0){//если координаты относительные - пересчитываем в абсолютные
				currentComand.posX = currentComand.posX + positionX;
				currentComand.posY = currentComand.posY + positionY;
				currentComand.posZ = currentComand.posZ + positionZ;
			}
			//есть два варианта команды:через радиус или через координаты центра
			//в первом случае координатами центра считаем центр хорды

			if(currentComand.valid & (1<<4)){//если строим через радиус

			}
			else{//если через координаты центра
				//получаем относительные координаты начальной точки интерполятора
				vPosX = positionX - currentComand.I;
				vPosY = positionY - currentComand.J;
			}

			calcInterpolation();
			break;
		}
		case(3):{//круговая интерполяция против часовой стрелки
			if(relCoordinates > 0){//если координаты относительные - пересчитываем в абсолютные
				currentComand.posX = currentComand.posX + positionX;
				currentComand.posY = currentComand.posY + positionY;
				currentComand.posZ = currentComand.posZ + positionZ;
			}
			//есть два варианта команды:через радиус или через координаты центра
			//в первом случае координатами центра считаем центр хорды

			if(currentComand.valid & (1<<4)){//если строим через радиус

			}
			else{//если через координаты центра
				//получаем относительные координаты начальной точки интерполятора
				vPosX = positionX - currentComand.I;
				vPosY = positionY - currentComand.J;
			}

			calcInterpolation();
			break;
		}
		case(28):{//перемещение до сработки концевиков
			calcInterpolation();
			break;
		}
		case(92):{//установка в нулевую позицию
			positionX=0;
			positionY=0;
			positionZ=0;
			comandExecuted();
			break;
		}
		case(90):{//установка абсолютных координат
			relCoordinates = 0;
			comandExecuted();
			break;
		}
		case(91):{//установка относительных координат
			relCoordinates = 1;
			comandExecuted();
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////
void calcInterpolation(){

	switch(currentComand.GCode){
		case(0):{
			if((positionX == currentComand.posX) && (positionY == currentComand.posY)){//если пришли в заданную позицию, то ничего не делаем
				comandExecuted();
				return;
			}
			break;
		}
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
		case(28):{//перемещение до сработки концевиков
			moveToZero();
			break;
		}
		default:{
			return;//todo потом сделать обработку ошибки
		}
	}
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
	if((positionX == currentComand.posX) &&
			(positionY == currentComand.posY) &&
			(positionZ == currentComand.posZ)){//если пришли в заданную позицию, то ничего не делаем
		comandExecuted();
		return;
	}

	int offsetX = zeroPointX - positionX;
	int offsetY = zeroPointY - positionY;
	int vectorX = currentComand.posX - positionX;
	int vectorY = currentComand.posY - positionY;

	int F = (abs(offsetY) * abs(vectorX) - (abs(offsetX) * abs(vectorY)));//оценочная функция
	if(F == 0){
		if(vectorX > 0){
			stepsCounterX = -1;
		}
		else if(vectorX < 0){
			stepsCounterX = 1;
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
			stepsCounterX = -1;
		}
		else if(vectorX < 0){
			stepsCounterX = 1;
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
				stepsCounterX = -1;
			}
			else if(vectorX < 0){
				stepsCounterX = 1;
			}
			else{
				stepsCounterX = 0;
			}
		}
	}
	HAL_TIM_Base_Start_IT(&htim2);//запускаем таймер
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void cwCicleInterpolation(){
	if((positionX == currentComand.posX) && (positionY == currentComand.posY)){//если пришли в заданную позицию, то ничего не делаем
		comandExecuted();
		return;
	}

	int F = ((vPosX * vPosX) + (vPosY * vPosY)) - (currentComand.R * currentComand.R);//оценочная функция (радиус всегда положительный)
																		//контролируется на этапе парсинга
	if(F >= 0){//если на окружности или за ней

		if((vPosX > 0) && (vPosY > 0)){//1 квадрант
			stepsCounterY = -1;
		}
		else if((vPosX > 0) && (vPosY < 0)){//2 квадрант
			stepsCounterX = 1;
		}
		else if((vPosX < 0) && (vPosY < 0)){//3 квадрант
			stepsCounterY = 1;
		}
		else if((vPosX < 0) && (vPosY > 0)){//4 квадрант
			stepsCounterX = -1;
		}
		else{
			if(vPosY > 0){
				stepsCounterY = -1;
			}
			else if(vPosY < 0){
				stepsCounterY = 1;
			}
			else if(vPosX > 0){
				stepsCounterX = 1;
			}
			else if(vPosX < 0){
				stepsCounterX = -1;
			}
		}
	}
	else if(F < 0){//если внутри окружности
		if((vPosX > 0) && (vPosY > 0)){//1 квадрант
			stepsCounterX = -1;
		}
		else if((vPosX > 0) && (vPosY < 0)){//2 квадрант
			stepsCounterY = -1;
		}
		else if((vPosX < 0) && (vPosY < 0)){//3 квадрант
			stepsCounterX = 1;
		}
		else if((vPosX < 0) && (vPosY > 0)){//4 квадрант
			stepsCounterY = 1;
		}
		else{
			if(vPosY > 0){
				stepsCounterX = -1;
			}
			else if(vPosY < 0){
				stepsCounterX = 1;
			}
			else if(vPosX > 0){
				stepsCounterY = -1;
			}
			else if(vPosX < 0){
				stepsCounterY = 1;
			}
		}
	}

	vPosX+=stepsCounterX;
	vPosY+=stepsCounterY;

	HAL_TIM_Base_Start_IT(&htim2);//запускаем таймер
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void ccwCicleInterpolation(){
	if((positionX == currentComand.posX) && (positionY == currentComand.posY)){//если пришли в заданную позицию, то ничего не делаем
		comandExecuted();
		return;
	}

	int F = ((vPosX * vPosX) + (vPosY * vPosY)) - (currentComand.R * currentComand.R);//оценочная функция (радиус всегда положительный)
																			//контролируется на этапе парсинга
		if(F >= 0){//если на окружности или за ней

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
					stepsCounterY = -1;
				}
				else if(vPosY < 0){
					stepsCounterY = 1;
				}
				else if(vPosX > 0){
					stepsCounterX = 1;
				}
				else if(vPosX < 0){
					stepsCounterX = -1;
				}
			}
		}
		else if(F < 0){//если внутри окружности
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
		vPosY+=stepsCounterY;

		HAL_TIM_Base_Start_IT(&htim2);//запускаем таймер
}
////////////////////////////////////////////////////////////////////////////////////////////
void moveToZero(){
	if((HAL_GPIO_ReadPin(swX) == GPIO_PIN_SET) &&
			(HAL_GPIO_ReadPin(swY) == GPIO_PIN_SET) &&
			(HAL_GPIO_ReadPin(swZ) == GPIO_PIN_SET)){
		positionX = 0;
		positionY = 0;
		positionZ = 0;
		comandExecuted();
		return;
	}

	if(HAL_GPIO_ReadPin(swX) != GPIO_PIN_SET){
		stepsCounterX = -1;
	}
	if(HAL_GPIO_ReadPin(swY) != GPIO_PIN_SET){
		stepsCounterY = -1;
	}
	if(HAL_GPIO_ReadPin(swZ) != GPIO_PIN_SET){
		stepsCounterZ = -1;
	}

	HAL_TIM_Base_Start_IT(&htim2);//запускаем таймер
}
/////////////////////////////////////////////////////////////////////////////////////
void comandExecuted(){
	vPosX = 0;
	vPosY = 0;
	counter = 0;
	resetCurrentCommand();
	sendFree();
}
//////////////////////////////////////////////////////////////////////////////////////
void resetCurrentCommand(){
	currentComand.F = 0;
	currentComand.GCode = 0;
	currentComand.I = 0;
	currentComand.J = 0;
	currentComand.R = 0;
	currentComand.posX = 0;
	currentComand.posY = 0;
	currentComand.posZ = 0;
	currentComand.valid = 0;
}
