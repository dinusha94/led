/**
  ******************************************************************************
  * @file           : app.h
  * @brief          : Header for app.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  */

#ifndef APP_H
#define APP_H

#include <math.h>
#include <stdlib.h>

/** Circle informations*/
#define RADIUS 4
#define CX 5
#define CY 1

/**
  * @brief: This function Initiate the application
  * @param: None
  * @return: None 
  */
void app_init(void);

/**
  * @brief: This function runs in the loop
  * @param:  None
  * @return: None 
  */
void app_run(void);

#endif
