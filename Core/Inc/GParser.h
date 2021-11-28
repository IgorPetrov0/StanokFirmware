/*
 * GParser.h
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Игорь
 */

#ifndef INC_GPARSER_H_
#define INC_GPARSER_H_

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "drive.h"


extern void executeGComand(struct driversComand comand);
extern void executeMComand(struct MComand comand);
extern struct Buffer rxBuffer;


void parseCodeString(char *array, char len);
void parseGCode(char *array, char len);
void parseMCode(char *array, char len);
unsigned char findSymbol(unsigned char *array, unsigned char len, char symbol,unsigned char start);
int stringToInt(char *string, char len, char *ok);
char isByteDigit(char byte);
int findInString(char *string, char symbol, char len);

#endif /* INC_GPARSER_H_ */
