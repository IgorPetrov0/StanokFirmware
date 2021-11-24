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

extern char incomingMessageFlag;
extern int positionX;
extern int positionY;
extern int positionZ;
extern void parseCodeString(unsigned char *array, char len);


enum messageType{
    MESSAGE_STATUS,
	MESSAGE_G_CODE
};



unsigned char *rxBuffer;
char sizeOfMessage;



void usbInit();
void readMessage();
unsigned short Crc16(char *pcBlock, unsigned short len);
void sendStatus();
void sendAcknolege(enum messageType mess, char *array, char arrSize);


#endif /* INC_USB_H_ */
