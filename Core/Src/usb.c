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
				sendAcknolege(MESSAGE_STATUS,NULL,0);
				break;
			}
			case(MESSAGE_G_CODE):{
				parseCodeString(rxBuffer+2,sizeOfMessage-4);
				//sendAcknolege(MESSAGE_ACKNOWLEDGE,rxBuffer+2,sizeOfMessage-4);
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
void status(char *array){
	//отправляем позицию X. Разбираем int на char
	memcpy(array,&positionX,sizeof(int));
	//отправляем позицию Y. Разбираем int на char
	memcpy(array+sizeof(int),&positionY,sizeof(int));
	//отправляем позицию X. Разбираем int на char
	memcpy(array+sizeof(int)*2,&positionZ,sizeof(int));

	if(HAL_GPIO_ReadPin(swX) == GPIO_PIN_SET){
		array[sizeof(int)*3]=1;
	}
	else{
		array[sizeof(int)*3]=0;
	}

	if(HAL_GPIO_ReadPin(swY) == GPIO_PIN_SET){
		array[sizeof(int)*3+1]=1;
	}
	else{
		array[sizeof(int)*3+1]=0;
	}

	if(HAL_GPIO_ReadPin(swZ) == GPIO_PIN_SET){
		array[sizeof(int)*3+2]=1;
	}
	else{
		array[sizeof(int)*3+2]=0;
	}
}
/////////////////////////////////////////////////////////////////////////
void sendAcknolege(enum messageType mess, char *array, char arrSize){
	unsigned char size=2+sizeof(int)*3+3+2;//размер,тип,CRC,размер статуса
	if(array != NULL){
		size += arrSize;//размер массива, если он есть
	}
	char dataToSend[size];

	dataToSend[0]=size;
	dataToSend[1]=mess;

	if(array != NULL){
		memcpy(array,dataToSend+2,arrSize);
	}
	status(dataToSend+2+arrSize);//статус в каждом ответном сообщении
	unsigned short crc = Crc16(dataToSend,size-2);
	memcpy(dataToSend+size-2,&crc,sizeof(unsigned short));
	CDC_Transmit_FS(dataToSend,size);//отправляем ответ
}
/////////////////////////////////////////////////////////////////////////
void sendFree(){
	sendAcknolege(MESSAGE_COMPLETE,NULL,0);
}

































