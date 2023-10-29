/*!
 * @file : bmp390.h
 *
 * Author: Yunus Emre KAYRA (github.com/YEK-Kayra)
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 TAISAT Turkish Artificial Intelligence Supported Autonomous Technologies
 *
 **/


/*
 * $WİLL BE ADDED FEATURES$
 *
 * !Chip Id , Revision Id will be added
 * !FIFO features and Sensor Time  will be added
 * Event register will be used for
 *
 **/

#ifndef INC_BMP390_H_
#define INC_BMP390_H_


/****************************************************************/
/*! @name       Header includes             */
/****************************************************************/
#include "main.h"
#include "stdbool.h"
#include "stdint.h"

/****************************************************************/
/*! @name       General Macro Definitions        			    */
/****************************************************************/

#define BMP390_REG_ERR		 		0x02  /*Sensor Error conditions ==> bits: conf_err[2:2], cmd_err[1:1], fatal_err[0:0], */
#define BMP390_REG_STATUS    		0x03  /*Sensor Status Flags	  ==> bits: drdy_temp[6:6], drdy_press[5:5], cmd_rdy[4:4]*/
#define BMP390_REG_DATA_0    		0x04  /*24Bit pressure data of XLSB byte*/
#define BMP390_REG_DATA_1    		0x05  /*24Bit pressure data of LSB 	byte*/
#define BMP390_REG_DATA_2    		0x06  /*24Bit pressure data of MSB 	byte*/
#define BMP390_REG_DATA_3    		0x07  /*24Bit temperature data of XLSB byte*/
#define BMP390_REG_DATA_4    		0x08  /*24Bit temperature data of LSB byte*/
#define BMP390_REG_DATA_5   		0x09  /*24Bit temperature data of MSB byte*/

#define BMP390_REG_PWR_CTRL   		0x1B  /**Enables or disables pressure and temperature measurement.
 	 	 	 	 	 	 	 	 	 	 	* bits : mode[5:4], temp_en[1:1], press_en[0:0]
 	 	 	 	 	 	 	 	 	 	 	* */

#define BMP390_REG_OSR   			0x1C  /**Controls the oversampling settings for pressure and temperature measurements.
 	 	 	 	 	 	 	 	 	 	 	* bits :  osr_t[5:3], osr_p[2:0]
 	 	 	 	 	 	 	 	 	 	 	* */

#define BMP390_REG_OSR   			0x1C  /**Set the configuration of the output data rates
 	 	 	 	 	 	 	 	 	 	 	* bits :  odr_sel[4:0]
 	 	 	 	 	 	 	 	 	 	 	* */







/******************************************************************************/
/*!  @name         External Variable 				                          */
/******************************************************************************/
extern double BMP390_Pressure;
extern double BMP390_Temperature;
extern float  BM390_Altitude;
extern float  BMP390_gForce;
extern float  BMP390_VerticalSpeed;

/******************************************************************************/
/*!  @name         Enum Declarations                                  		  */
/******************************************************************************/




/******************************************************************************/
/*!  @name         Structure Declarations                             		  */
/******************************************************************************/

/******************************************************************************/
/*         			BMP280 Function prototypes            				  	  */
/******************************************************************************/

//BMP390_StatusTypeDef BMP390_Init();





#endif /* INC_BMP390_H_ */
