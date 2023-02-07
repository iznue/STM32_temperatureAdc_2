/*
 * uart.c
 *
 *  Created on: Feb 3, 2023
 *      Author: npedu032
 */

#include "uart.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

#define rxBufferMax 255
int rxBufferGp;//문자 수신을 받는 용도의 버퍼 //get pointer (read)
int rxBufferPp;											//put pointer (write)
uint8_t rxBuffer[rxBufferMax];
uint8_t rxChar;

//ring buffer
//uart 장치 초기화
void initUart(){
	HAL_UART_Receive_IT(&huart1, &rxChar, 1); // 수신이 되면 INTERRUPT 발생, 발생 시 rxChar에 1글자 넣음, rxChar 주소 할당
}

//문자수신 처리
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){ // interrupt 처리
	rxBuffer[rxBufferPp++] = rxChar; // putpoint 증가, 버퍼가 255를 넘으면 오버플로우됨
	rxBufferPp %= rxBufferMax; // 이를 방지하기 위함
	HAL_UART_Receive_IT(&huart1, &rxChar, 1); //interrupt 재설정
}

//버퍼에서 문자를 꺼내옴 = scanf의 역할
uint8_t getChar(){
	uint8_t result; // 255를 넘었을 때 초기화하기 위해 result 지정
	if(rxBufferGp == rxBufferPp)return 0;
	result = rxBuffer[rxBufferGp++];
	rxBufferGp %= 255; // max를 벗어나는 경우 나눠줌
	return result;
}

int _write(int file, char *p, int len){
	HAL_UART_Transmit(&huart1, p, len, 1);
	return len;
}
