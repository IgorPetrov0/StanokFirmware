/*
 * GParser.c
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Игорь
 */

#include "GParser.h"


void parseCodeString(char *array, char len){
	switch(array[2]){
		case('G'):{
			parseGCode(array+3,len-1);
			break;
		}
		case('M'):{
			parseMCode(array+3,len-1);
			break;
		}
		default:{

			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////
void parseGCode(char *array, char len){
	//в массиве строка в коде ASCII. первые 2 байта команда G, далее аргументы через пробелы

	struct driversComand comand;
	comand.GCode=0;
	comand.posX=0;
	comand.posY=0;
	comand.posZ=0;
	comand.F=0;
	comand.R=0;
	comand.valid=0;

	char ok=0;
	float result=0;
	int pos=-1;

	//читаем номер команды
	int tmp=(int)(array[0]);
	if(isdigit(tmp)!=0){//первый символ в массиве должен быть числом. Символ G отстрелили в вызывающей функции
		comand.GCode=(char)(stringToInt(array,2,&ok)/100);
	}
	else{//иначе ошибка. todo потом добавить обработку ошибки
		return;
	}

	//читаем координату Х
	pos = findInString(array,'X',len);
	if(pos!=-1){
		result = stringToInt(&array[pos]+1,len-pos,&ok);
		if(ok!=0){
			comand.posX = result/10;
			comand.valid |= (1<<0);
		}
	}

	//читаем координату Y
	pos = findInString(array,'Y',len);
	if(pos!=-1){
		result = stringToInt(&array[pos]+1,len-pos,&ok);
		if(ok!=0){
			comand.posY = result/10;
			comand.valid |= (1<<1);
		}
	}

	//читаем координату Z
	pos = findInString(array,'Z',len);
	if(pos!=-1){
		result = stringToInt(&array[pos]+1,len-pos,&ok);
		if(ok!=0){
			comand.posZ = result/10;
			comand.valid |= (1<<2);
		}
	}

	//читаем F
	pos = findInString(array,'F',len);
	if(pos!=-1){
		result = stringToInt(&array[pos]+1,len-pos,&ok);
		if(ok!=0){
			comand.F = result;
			comand.valid |= (1<<3);
		}
	}

	//читаем R
	pos = findInString(array,'R',len);
	if(pos!=-1){
		result = stringToInt(&array[pos]+1,len-pos,&ok);
		if(ok!=0){
			if(result < 0){//радиус не может быть отрицательным
				result = 0 - result;
			}
			comand.R = result/10;
			comand.valid |= (1<<4);
		}
	}

	//читаем I
	pos = findInString(array,'I',len);
	if(pos!=-1){
		result = stringToInt(&array[pos]+1,len-pos,&ok);
		if(ok!=0){
			comand.I = result/10;
			comand.valid |= (1<<5);
		}
	}

	//читаем J
	pos = findInString(array,'J',len);
	if(pos!=-1){
		result = stringToInt(&array[pos]+1,len-pos,&ok);
		if(ok!=0){
			comand.J = result/10;
			comand.valid |= (1<<6);
		}
	}




	executeGComand(comand);
}
////////////////////////////////////////////////////////////////////////
void parseMCode(char *array, char len){
	struct MComand comand;
	char ok=0;

	//читаем номер команды
	int tmp=(int)(array[0]);
	if(isdigit(tmp)!=0){//первый символ в массиве должен быть числом. Символ G отстрелили в вызывающей функции
		comand.MCode=(char)(stringToInt(array,2,&ok)/100);
		executeMComand(comand);
	}
	else{//иначе ошибка. todo потом добавить обработку ошибки
		return;
	}
}
//////////////////////////////////////////////////////////////////////////
int stringToInt(char *string, char len, char *ok){
	int endPos=0;
	int pointPos=len;
	int result=0;
	char minus=0;

	for(; endPos!=len; endPos++){//получаем индекс окончания первого числа в строке
		if(isByteDigit(string[endPos]) == 0){
			if(string[endPos] == 46){//проверяем на десятичную точку (46 - ASCII код точки)
				if(pointPos == len){//если раньше точка не встречалась
					pointPos=endPos;//то запоминаем позицию
				}
				else{//если на точку уже натыкались, то в строке ошибка
					*ok=0;
					return 0;
				}
			}
			else if(string[endPos] == 45){//если минус (45 - ASCII код минуса)
				if(minus != 0){//если минус уже встречался, то в строке ошибка
					*ok=0;
					return 0;
				}
				minus=1;
			}
			else if(string[endPos] == 32){//если пробел (32 - ASCII код пробела)
				break;//то прерываемся
			}
			else if(string[endPos] == 10){//если конец строки (10 - ASCII код конца строки)
				break;//то прерываемся
			}
			else{//если символ не цифра и не точка, то в строке ошибка
				*ok=0;
				return 0;
			}
		}
	}
	if(pointPos==len){//точка в строке так и не встретилась,
		pointPos=endPos;//то её позиция в конце числа
	}


	if(endPos!=0){
		float multiplier=100;
		for(int n=pointPos; n!=minus; n--){//двигаемся влево от позиции точки
			result += (string[n-1]-48)*multiplier;
			multiplier*=10;
		}

		multiplier=10;
		for(int n=pointPos+1; n<endPos; n++){//двигаемся в право от позиции точки на 2 знака
			result += (string[n]-48)*multiplier;
			multiplier*=0.1;
		}
	}
	else{
		*ok=0;
		return 0;
	}
	if(minus != 0){
		result = 0 - result;
	}
	*ok=1;
	return result;
}
//////////////////////////////////////////////////////////////////////////////////
char isByteDigit(char byte){
	if((byte >=48) && (byte<=57)){
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
int findInString(char *string, char symbol, char len){
	for(int n=0; n!=len; n++){
		if(string[n] == symbol){
			return n;
		}
	}
	return -1;
}
