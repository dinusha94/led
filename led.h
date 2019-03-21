/**
  ******************************************************************************
  * @file           : led.h
  * @brief          : Header for led.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

#ifndef LED_H
#define LED_H

#include "stm32f1xx_hal.h"

/**Enumaration for led patterns*/
typedef enum _LED_PTRN_t{
	LED_PTRN_OFF = 0,
	LED_PTRN_ON,
	LED_PTRN_BLINK,
}LED_PTRN_t;


/**
  * @brief: This function Initiate the led driver
  * @param: None
  * @return: None 
  */
void led_init(void);

/**
  * @brief: This function change the led patterns
  * @param: led pattern, blink parameter (number of times to blink)
  * @return: None 
  */
void led_set_pattern(LED_PTRN_t pattern, uint8_t pattern_parm);
#endif

