/*
 * uart.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Galaxy
 */
#include "uart.h"

uint8_t buffer[MAX_BUFFER_SIZE] = "!ADC=";
uint8_t index_buffer = 0;
uint8_t flagSendData = 0;
uint8_t status = WAIT_HEADER;
uint16_t ADC_value = 0;
uint8_t temp = '0';

char str[4] = "0000";
enum comState
{
	WAIT_COMMAND,
	SEND_DATA,
	RESEND_DATA
};
enum comState statusOfCom = WAIT_COMMAND;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		HAL_UART_Transmit (&huart2 , &temp , 1, 50);
		HAL_UART_Receive_IT(&huart2, &temp, 1);
		command_parser_fsm();
		uart_communication_fsm();
	}
}

void uart_communication_fsm()
{
	switch (statusOfCom)
	{
	case WAIT_COMMAND:
		if (flagSendData)
		{
			statusOfCom = SEND_DATA;
			// Reading ADC
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 100);
			ADC_value = HAL_ADC_GetValue(&hadc1);
			// Convert to string and print
			sprintf(str, "%hu", ADC_value);
		}
		break;
	case SEND_DATA:
		// Send data to UART
		HAL_UART_Transmit(&huart2, buffer, 5, 100);
		HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);
		HAL_UART_Transmit(&huart2, (uint8_t *)"#\r\n", 3, 100);
		if (flagSendData)
		{
			statusOfCom = RESEND_DATA;
			setTimer1(1000);
		}
		else
			statusOfCom = WAIT_COMMAND;
		break;
	case RESEND_DATA:
		if (is_flag1())
		{
			// Send data to UART
			HAL_UART_Transmit(&huart2, buffer, 5, 100);
			HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 100);
			HAL_UART_Transmit(&huart2, (uint8_t *)"#\r\n", 3, 100);
			setTimer1(1000);
		}
		if (!flagSendData)
		{
			statusOfCom = WAIT_COMMAND;
			setTimer1(100);
		}
		break;
	default:
		statusOfCom = WAIT_COMMAND;
	}
}

void command_parser_fsm(void)
{
	switch (status)
	{
	case INIT_RECEIVE:
		status = WAIT_HEADER;
		break;
	case WAIT_HEADER:
		if (temp == '!')
			status = RECEIVE_DATA;
		break;
	case RECEIVE_DATA:
		if (temp == 'R')
			status = RECEIVE_DATA_RST_2;
		else if (temp == 'O')
			status = RECEIVE_DATA_OK_2;
		else
			status = WAIT_HEADER;
		break;
	case RECEIVE_DATA_RST_2:
		if (temp == 'S')
			status = RECEIVE_DATA_RST_3;
		else
			status = WAIT_HEADER;
		break;
	case RECEIVE_DATA_RST_3:
		if (temp == 'T')
			status = END_DATA_RST;
		else
			status = WAIT_HEADER;
		break;
	case RECEIVE_DATA_OK_2:
		if (temp == 'K')
			status = END_DATA_OK;
		else
			status = WAIT_HEADER;
		break;
	case END_DATA_RST:
		if (temp == '#')
			flagSendData = 1;
		status = WAIT_HEADER;
		break;
	case END_DATA_OK:
		if (temp == '#')
			flagSendData = 0;
		status = WAIT_HEADER;
		break;
	default:
		status = INIT_RECEIVE;
		break;
	}
}

