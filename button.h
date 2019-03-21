/**
  ******************************************************************************
  * @file           : button.h
  * @brief          : Header for button.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f1xx_hal.h"

/**Enumaration for button id*/
typedef enum _BUTTON_ID_t{
	BTN_ID_1 = 0,
}BUTTON_ID_t;

/**Enumaration for button action*/
typedef enum _BUTTON_ACTION_t{
	BTN_ACTION_PRESSED = 0,
	BTN_ACTION_RELEASED,
}BUTTON_ACTION_t;


/**
  * @brief: Function pointer to on_button_pressed function
  * @param: None
  * @return: None 
  */
typedef void (*BTN_CALLBACK_t)(BUTTON_ID_t button_id, BUTTON_ACTION_t action);

/**
  * @brief: This function initiate the button driver
  * @param: None
  * @return: None 
  */
void btn_init(void);

/**
  * @brief: This function subscribe button callbacks
  * @param: None
  * @return: None 
  */
void btn_subscribe(BTN_CALLBACK_t cb);

#endif
