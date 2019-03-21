/**
  ******************************************************************************
  * @file           : packet_handler.h
  * @brief          : Header for packet_handler.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

#ifndef PACKET_H
#define PACKET_H

#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

/** CRC */
#define POLY 0x1021
#define CRC_INIT 0xFFFF

/** 4 Starting bytes*/
#define STX1 0x02
#define STX2 0x02
#define STX3 0x02
#define STX4 0x02

/** Signature data*/
#define SIGA 0x03 // SIAGA means user is giving a command to on/off led
#define SIGB 0x04 // SIGB means user ask for a point in the circleled.h

/** Serial packet sructure */
typedef struct _SERIAL_PACKET_t{
	
 uint8_t sb1;
 uint8_t sb2;
 uint8_t sb3;
 uint8_t sb4;
 uint8_t sig;
 uint8_t length;
 uint8_t data[10]; 
 uint8_t low_crc;
 uint8_t high_crc;
	
}SERIAL_PACKET_t ;


/**
* @brief  packet collector application status are stored in the structure defined bellow
*/
typedef enum _PACKET_COLLECTOR_STATE_t{
    /** Waiting for start byte 1 */
    PACKET_COLLECTOR_STATE_STATE_STX_1 = 0,
    /** Waiting for start byte 2 */
    PACKET_COLLECTOR_STATE_STATE_STX_2,
    /** Waiting for start byte 3 */
    PACKET_COLLECTOR_STATE_STATE_STX_3,
    /** Waiting for start byte 4 */
    PACKET_COLLECTOR_STATE_STATE_STX_4,
    /** Waiting for packet signature 1 */
    PACKET_COLLECTOR_STATE_PKT_SIG,
    /** Waiting for packet type */
    PACKET_COLLECTOR_STATE_TYPE,
    /** Waiting for low byte of packet length */
    PACKET_COLLECTOR_STATE_STATE_LENGTH,
    /** Collecting data bytes */
    PACKET_COLLECTOR_STATE_STATE_DATA,
    /** Waiting for low byte of 16bit CRC */
    PACKET_COLLECTOR_STATE_STATE_CRC_L,
    /** Waiting for high byte of 16bit CRC */
    PACKET_COLLECTOR_STATE_STATE_CRC_H,
}PACKET_COLLECTOR_STATE_t;


/**
  * @brief: This function is use to get the received packet from packer driver
  * @param: None
  * @return: received packet[See  SERIAL_PACKET_t]
  */
SERIAL_PACKET_t get_packet(SERIAL_PACKET_t* pack);

/**
  * @brief: This function is use to check is there a valied packet is in the packet handler driver
  * @param: None
  * @return: None
  */
bool is_packet_complete(void);

/**
  * @brief: This function is use to get the received packet from packer driver
  * @param:  Result to return to the user
  * @return: None
  */
void packet_transmit(uint8_t* responce);

/**
  * @brief: This function initiate the packet handler
  * @param: None
  * @return: None
  */
void packet_handler_init(void);
#endif

