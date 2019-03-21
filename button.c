/**
  ******************************************************************************
  * @file           : button.c
  * @brief          : Button driver file
  ******************************************************************************
  */
	
#include "button.h"

//-----------------------------------------------------
//                  Global variables
//-----------------------------------------------------

BTN_CALLBACK_t g_btn_app_cb;

//-----------------------------------------------------
//                 Function Definitions
//-----------------------------------------------------

/**
	* @attention    : Definetions of private functions of the button.c driver module
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

//-----------------------------------------------------
//                    Public functions
//-----------------------------------------------------

/**
  * @brief        : This function initiate button driver
	* @attention    : Since we use cubemx no need to setup GPIO pins in the driver
  */
void btn_init(void)
{
	
}

/**
  * @brief : This function register the button callback events
	* 
  */
void btn_subscribe(BTN_CALLBACK_t cb)
{
	g_btn_app_cb = cb;
}

//-----------------------------------------------------
//                   Private functions
//-----------------------------------------------------

/**
  * @brief        : Externel interrupt callback function provided by HAL layer
	* 
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == 1)
	{
		BUTTON_ACTION_t action = BTN_ACTION_RELEASED;
		GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
		if(state == GPIO_PIN_SET)
		{
			action = BTN_ACTION_PRESSED;
		}
		
		if(g_btn_app_cb  != 0)
		{
			g_btn_app_cb(BTN_ID_1, action);
		}
	}
}

