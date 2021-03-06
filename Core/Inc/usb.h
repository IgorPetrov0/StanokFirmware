/*
 * usb.h
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Игорь
 */

#ifndef INC_USB_H_
#define INC_USB_H_

#include <string.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"


#define swX GPIOA, GPIO_PIN_0
#define swY GPIOA, GPIO_PIN_1
#define swZ GPIOA, GPIO_PIN_2

extern char incomingMessageFlag;
extern int positionX;
extern int positionY;
extern int positionZ;
extern void parseCodeString(char *array, char len);


enum messageType{
	MESSAGE_STATUS,
	MESSAGE_G_CODE,
	MESSAGE_COMPLETE,
	MESSAGE_ACKNOWLEDGE,
	MESSAGE_ERROR
};



char *rxBuffer;
char sizeOfMessage;



void usbInit();
void readMessage();
unsigned short Crc16(char *pcBlock, unsigned short len);
void status(char *array);
void sendAcknolege(enum messageType mess, char *array, char arrSize);
void sendFree();



#endif /* INC_USB_H_ */
