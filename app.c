/**
  ******************************************************************************
  * @file           : app.c
  * @brief          : Application layer file
  ******************************************************************************
  */
	
#include "app.h"
#include "button.h"
#include "led.h"
#include "packet_handler.h"

//-----------------------------------------------------
//                  Global variables
//-----------------------------------------------------

SERIAL_PACKET_t g_serial_packet;
uint8_t g_result = 1;
bool x;

//-----------------------------------------------------
//                 Function Definitions
//-----------------------------------------------------

/**
	* @attention    : Definetions of private functions of the app.c module
  */
void on_button_pressed(BUTTON_ID_t button_id, BUTTON_ACTION_t action);
uint8_t process_packet(SERIAL_PACKET_t* pack, uint8_t* result);
uint8_t calculate_position(uint8_t x, uint8_t y);

//-----------------------------------------------------
//                    Public functions
//-----------------------------------------------------

 /**
  * @brief: This function initiate button and led drivers and handlle button callback events
  */
void app_init(void)
{
	btn_init();
	btn_subscribe(on_button_pressed);
	led_init();
}



void app_run(void)
{
	/*Handle packets */
	//x = is_packet_complete();
	
	//if(x == true)
	//{
		 // x = false;
		 // get_packet(&g_serial_packet);
		
	  //  process_packet(&g_serial_packet, &g_result);
		
		  packet_transmit(&g_result);
		
	//}
	
}


//-----------------------------------------------------
//                   Private functions
//-----------------------------------------------------

/**
  * @brief: Private function in the button driver which handlle the button events
  */
void on_button_pressed(BUTTON_ID_t button_id, BUTTON_ACTION_t action)
{
	led_set_pattern(LED_PTRN_BLINK,3);
}


/**
  * @brief: Private function process the packets
  */
uint8_t process_packet(SERIAL_PACKET_t* pack, uint8_t* result)
{
	
	switch((*pack).sig )
	{
		case SIGA:
			  /**Do the led stuf*/
		
		    if((*pack).data[0] == 0x01){
					led_set_pattern(LED_PTRN_ON,0);
				}
				else{
					led_set_pattern(LED_PTRN_OFF,0);
				}
			break;
		
		case SIGB:
			/**Do the circle stuff*/
		  
		 *result = calculate_position((*pack).data[0] , (*pack).data[1] );
		
			break;
		
		default:
			break;
	}
	 return *result;
}


uint8_t calculate_position(uint8_t x, uint8_t y)
{
  uint8_t res;
  int dist = sqrt(pow(abs(CX - x),2)+ pow(abs(CY - y),2));

  if(dist < RADIUS)
  {
      res = 1;
  }
  else
  {
      res = 0;
  }

  return res;
}

