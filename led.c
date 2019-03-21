/**
  ******************************************************************************
  * @file           : led.c
  * @brief          : LED driver file
  ******************************************************************************
  */
	
#include "led.h"
#include "tim.h"

//-----------------------------------------------------
//                  Global variables
//-----------------------------------------------------

LED_PTRN_t g_led_pattern;

uint8_t g_pattern_parm;

//-----------------------------------------------------
//                 Function Definitions
//-----------------------------------------------------

/**
	* @attention    : Definetions of private functions of the led.c driver module
  */
void led_pattern_statemachine(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


//-----------------------------------------------------
//                    Public functions
//-----------------------------------------------------

/**
  * @brief        : This function initiate led driver
	* @attention    : this also startthe timer interrupt on timer3
  */
void led_init(void)
{
	g_led_pattern = LED_PTRN_OFF;
	HAL_TIM_Base_Start_IT(&htim3);
	
}


/**
  * @brief        : This function sets the led patterns
  * @params       : patern type  & parameters related to the paterns
  */

void led_set_pattern(LED_PTRN_t pattern, uint8_t pattern_parm)
{
	g_led_pattern = pattern;
	
	switch(g_led_pattern)
	{
		case LED_PTRN_BLINK:
			g_pattern_parm = pattern_parm * 2;
			break;
		
		default:
			g_pattern_parm = pattern_parm;
			break;
	}	
}

//-----------------------------------------------------
//                   Private functions
//-----------------------------------------------------

/**
  * @brief  : State machine to set the different led paterns
	* 
  */

void led_pattern_statemachine(void)
{
	switch(g_led_pattern)
	{
		case LED_PTRN_OFF:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
			break;
		
		case LED_PTRN_ON:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			break;
		
		case LED_PTRN_BLINK:
			if(g_pattern_parm > 0)
			{
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
				g_pattern_parm--;
			}
			else
			{
				g_led_pattern = LED_PTRN_OFF;
			}
			break;
		
		default:
			g_led_pattern = LED_PTRN_OFF;
			break;
	}
}


/**
  * @brief        : Timer interrupt callback function provided by HAL layer
	* 
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	
	if(htim->Instance==TIM3){
		led_pattern_statemachine();
	}	
}
