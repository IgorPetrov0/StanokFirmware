/*
 * usb.c
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Игорь
 */


#include "usb.h"

void usbInit(){
	if(rxBuffer==NULL){
		rxBuffer=malloc(50);
	}
	sizeOfMessage=0;
}
///////////////////////////////////////////////////////////////
void readMessage(){

	incomingMessageFlag=0;
	unsigned short crc=0;
	unsigned short crc2=0;
	memcpy(&crc,&rxBuffer[sizeOfMessage-2],2);
	crc2=Crc16(rxBuffer,sizeOfMessage-2);
	if(crc==crc2){
		switch(rxBuffer[1]){
			case(MESSAGE_STATUS):{
				sendStatus();
				break;
			}
			case(MESSAGE_G_CODE):{
				parseCodeString(rxBuffer+2,sizeOfMessage-4);
				sendAcknolege(MESSAGE_G_CODE,rxBuffer+2,sizeOfMessage-4);
				break;
			}
		}
	}

}
////////////////////////////////////////////////////////////////
unsigned short Crc16(char *pcBlock, unsigned short len){
    unsigned short crc = 0xFFFF;
    unsigned char i;

    while (len--){
        crc ^= *pcBlock++ << 8;
        for (i = 0; i < 8; i++){
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }
    return crc;
}
////////////////////////////////////////////////////////////////////////
void sendStatus(){
	unsigned char size=2+sizeof(int)*3+2;//размер,тип,3 флоата,CRC
	unsigned char dataToSend[size];

	dataToSend[0]=size;
	dataToSend[1]=MESSAGE_STATUS;
	//отправляем позицию X. Разбираем float на char
	memcpy(dataToSend+2,&positionX,sizeof(int));
	//отправляем позицию Y. Разбираем float на char
	memcpy(dataToSend+2+sizeof(int),&positionY,sizeof(int));
	//отправляем позицию X. Разбираем float на char
	memcpy(dataToSend+2+sizeof(int)*2,&positionZ,sizeof(int));

	unsigned short crc = Crc16(dataToSend,size-2);
	memcpy(dataToSend+size-2,&crc,sizeof(unsigned short));

	CDC_Transmit_FS(dataToSend,size);//отправляем ответ

}
/////////////////////////////////////////////////////////////////////////
void sendAcknolege(enum messageType mess, char *array, char arrSize){
	unsigned char size=2+sizeof(int)*3+2+arrSize;//размер,тип,3 флоата,CRC, размер массива
	unsigned char dataToSend[size];

	dataToSend[0]=size;
	dataToSend[1]=mess;
	memcpy(array,dataToSend+2,arrSize);
	unsigned short crc = Crc16(dataToSend,size-2);
	memcpy(dataToSend+size-2,&crc,sizeof(unsigned short));
	CDC_Transmit_FS(dataToSend,size);//отправляем ответ
}


































