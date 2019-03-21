/**
  ******************************************************************************
  * @file           : packet_handler.c
  * @brief          : Application layer file
  ******************************************************************************
  */
	
#include "packet_handler.h"

//-----------------------------------------------------
//                  Global variables
//-----------------------------------------------------

/** size of the buffer to hold the transmit data*/
uint8_t rx_buffer;
uint8_t tx_buffer[12];

bool flag,tx_compleate;

/** local variable to hold packet collector states*/
PACKET_COLLECTOR_STATE_t g_packet_collector_state;

uint16_t crc,crc_calc,crc_tx;
uint8_t crc_l,crc_h;
uint8_t length;

SERIAL_PACKET_t tx_packet;
SERIAL_PACKET_t rx_packet;

//-----------------------------------------------------
//                 Function Definitions
//-----------------------------------------------------

SERIAL_PACKET_t packet_maker( uint8_t* buff, SERIAL_PACKET_t* packet);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint16_t CRC_calc(uint8_t data_byte,uint16_t CalCRC);
uint16_t crcGenarator(uint8_t * str, uint16_t len);
//-----------------------------------------------------
//                    Public functions
//-----------------------------------------------------

void packet_handler_init()
{
	
}


SERIAL_PACKET_t get_packet(SERIAL_PACKET_t* pack)
{
	
	return rx_packet;

}


/**
  * @brief: This function checks if there is a complete packet in the packet handler
  */
bool is_packet_complete()
{

	bool x;
	
	if(flag){
		x = true;
	}
	else{
		x = false;
	}
	
	return x;
}

/**
  * @brief: This function transmit the responce packet
  */
void packet_transmit(uint8_t* res)
{
	int i = 0;
	uint8_t len = sizeof(*res);
	 
	tx_buffer[i] = STX1; i++;
	tx_buffer[i] = STX2; i++;
	tx_buffer[i] = STX3; i++;
	tx_buffer[i] = STX4; i++;
	tx_buffer[i] = SIGB; i++;
	tx_buffer[i] = len; i++;
	
	if(*res == 1){  // sent in
	tx_buffer[i] = 0x6f; i++;
	tx_buffer[i] = 0x6e; i++;
	}
	
	else if(*res == 0){  // sent out
	tx_buffer[i] = 0x6f; i++;
	tx_buffer[i] = 0x66; i++;
	tx_buffer[i] = 0x66; i++;
	}
	
	crc_tx = crcGenarator(tx_buffer, 9);
	
	crc_l = crc_tx & 0xff;
	crc_h = crc_tx >> 8;
	
	tx_buffer[i] = crc_l; i++;
	tx_buffer[i] = crc_h; i++;
	
	for(int j =0; j<i;j++)
	{
		if(tx_compleate == true){
		HAL_UART_Transmit(&huart1, &tx_buffer[j], 1, 2);
			tx_compleate = false;
		}
		
	}
	i = 0;
}


//-----------------------------------------------------
//                   Private functions
//-----------------------------------------------------

SERIAL_PACKET_t packet_maker( uint8_t* buff, SERIAL_PACKET_t* packet)
{
	
	
	switch(g_packet_collector_state)
	{
		
		case PACKET_COLLECTOR_STATE_STATE_STX_1:
			if(*buff == STX1){
			g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_2;
			}
			break;
			
		case PACKET_COLLECTOR_STATE_STATE_STX_2:	
			if(*buff == STX2){
			g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_3;
			}
			break;
			
		case PACKET_COLLECTOR_STATE_STATE_STX_3:	
			if(*buff == STX3){
			g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_4;
			}
			break;
			
		case PACKET_COLLECTOR_STATE_STATE_STX_4:	
			if(*buff == STX4){
			g_packet_collector_state = PACKET_COLLECTOR_STATE_PKT_SIG;
			}
			break;	
			
		case PACKET_COLLECTOR_STATE_PKT_SIG:	
			if(*buff == SIGA){
			  (*packet).sig = SIGA;
			}
			else if(*buff == SIGB){
				(*packet).sig = SIGB;
			}
			
			g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_LENGTH;
			break;
			
		case PACKET_COLLECTOR_STATE_STATE_LENGTH:
				 length = *buff ;
		     g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_DATA;
			break;
			
    case PACKET_COLLECTOR_STATE_STATE_DATA:
			   for(int i =0; i < length; i++)
	       	{
					    (*packet).data[i] = *buff;
					}
			   g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_CRC_L;
			break;
			
    case PACKET_COLLECTOR_STATE_STATE_CRC_L:
			crc_l = *buff;
		  g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_CRC_H;
		  break;
		
		
   case PACKET_COLLECTOR_STATE_STATE_CRC_H:
			crc_h = *buff;
	    
      /** 16 bit crc number received*/	 
	    crc = (crc_l << 8) | (crc_h & 0xff);
	    //CRC_calc(crc,crc_calc);
	    
	    if(crc == crc_calc){
				
		          flag = true;
				
				      
	        }
	    else{
	      	    flag = false;
	        }
			
		  g_packet_collector_state = PACKET_COLLECTOR_STATE_STATE_STX_1;
		  break;

   default:
			break;
			
	}
	return *packet;
}



/**
  * @brief: This function calculate the crc
  */
uint16_t CRC_calc(uint8_t data_byte,uint16_t CalCRC) 
{
		uint16_t j = 0;

		CalCRC = CalCRC ^ (data_byte << 8); // n = 16 in this example
			for (j = 0; j < 8; j++) // Assuming 8 bits per byte
				{ 
					if (CalCRC & 0x8000)
						{ // if leftmost (most significant) bit is set
							CalCRC = (CalCRC << 1) ^ POLY;
						} 
					else 
						{
								CalCRC = CalCRC << 1;
						}

					CalCRC = CalCRC & CRC_INIT; // Trim remainder to 16 bits
				}
			return CalCRC;
}


/**
  * @brief: This function Genarate the crc
  */
uint16_t crcGenarator(uint8_t * str, uint16_t len)
{
	uint16_t rem = CRC_INIT;
	uint16_t i = 0;
	uint16_t j = 0;
// A popular variant complements rem here
	for (i = 0; i < len; i++)
		{
			rem = rem ^ (str[i] << 8); // n = 16 in this example
			for (j = 0; j < 8; j++) // Assuming 8 bits per byte
					{ 
						if (rem & 0x8000)
								{ // if leftmost (most significant) bit is set
									rem = (rem << 1) ^ POLY;
								} 
						else 
								{
									rem = rem << 1;
								}	
					rem = rem & CRC_INIT; // Trim remainder to 16 bits
				}
		}
// A popular variant complements rem here
			return rem;
}



/**
  * @brief: UART TXcomplete callback
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
 tx_compleate = true;
}

/**
  * @brief: UART RXcomplete callback
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
  packet_maker( &rx_buffer, &rx_packet);
	
}
