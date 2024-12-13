/*
 * timer.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Galaxy
 */


#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

void setTimer0(int duration);
void setTimer1(int duration);
void timerRun();
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim);

unsigned char is_flag0();
unsigned char is_flag1();

#endif /* INC_TIMER_H_ */

