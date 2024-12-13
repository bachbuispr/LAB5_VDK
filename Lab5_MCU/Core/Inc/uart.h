/*
 * uart.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Galaxy
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "timer.h"

extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;

#define MAX_BUFFER_SIZE	30
#define INIT_RECEIVE	0
#define WAIT_HEADER		1
#define RECEIVE_DATA	2
#define RECEIVE_DATA_RST_2	3
#define RECEIVE_DATA_RST_3	4
#define END_DATA_RST 5
#define RECEIVE_DATA_OK_2 6
#define END_DATA_OK 7

extern uint8_t temp;
extern char str[4];

extern uint8_t status;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t flagSendData;
extern uint16_t ADC_value;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
void uart_communication_fsm();
void command_parser_fsm (void);

#endif /* INC_UART_H_ */
