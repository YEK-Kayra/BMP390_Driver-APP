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
 * Event register will be used
 * CMD register will be used
 **/

#ifndef INC_BMP390_H_
#define INC_BMP390_H_


/****************************************************************/
/*!@name         Header includes            				    */
/****************************************************************/
#include "main.h"
#include "stdbool.h"
#include "stdint.h"

/******************************************************************************/
/*!@name          BMP390 External Variable	     	                          */
/******************************************************************************/
extern double BMP390_Pressure;
extern double BMP390_Temperature;
extern float  BM390_Altitude;
extern float  BMP390_gForce;
extern float  BMP390_VerticalSpeed;


/****************************************************************/
/*!@name          BMP390 Registers   			    		    */
/****************************************************************/

#define BMP390_REG_ERR		 		0x02  /*Sensor Error conditions ==> bits: conf_err[2:2], cmd_err[1:1], fatal_err[0:0] */
#define BMP390_REG_STATUS    		0x03  /*Sensor Status Flags ==> bits: drdy_temp[6:6], drdy_press[5:5], cmd_rdy[4:4] (drdy:Data ready x)*/
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
 	 	 	 	 	 	 	 	 	 	 	* bits :  osr_t[5:3](for temperature), osr_p[2:0](for pressure)
 	 	 	 	 	 	 	 	 	 	 	* */

#define BMP390_REG_ODR   			0x1D  /**Set the configuration of the output data rates
 	 	 	 	 	 	 	 	 	 	 	* bits :  odr_sel[4:0]
 	 	 	 	 	 	 	 	 	 	 	* */

#define BMP390_REG_CONFIG		 	0x1F /**Controls the IIR filter coefficients.
 	 	 	 	 	 	 	 	 	 	 	* bits :  iir_filter[3:1]
 	 	 	 	 	 	 	 	 	 	 	* */


/******************************************************************************/
/*!@name         	BMP390 Enums                                      		  */
/******************************************************************************/

/**
 * @brief BMP390 IIR filter coefficients enumeration definition
 */
typedef enum{

	BMP390_Filter_Coef_X0 =   0,   /* filter coefficient : 0 */
	BMP390_Filter_Coef_X1 =   1,   /* filter coefficient : 1 */
	BMP390_Filter_Coef_X3 =   2,   /* filter coefficient : 3 */
	BMP390_Filter_Coef_X7 =   3,   /* filter coefficient : 7 */
	BMP390_Filter_Coef_X15 =  4,   /* filter coefficient : 15 */
	BMP390_Filter_Coef_X31 =  5,   /* filter coefficient : 31 */
	BMP390_Filter_Coef_X63 =  6,   /* filter coefficient : 63 */
	BMP390_Filter_Coef_X127 = 7    /* filter coefficient : 127 */

}BMP390_FilterCoefficients;

/******************************************************************************/
/*!@name         	BMP390 Structures                                         */
/******************************************************************************/

/******************************************************************************/
/*!@name          	BMP390 Function Prototypes            			  		  */
/******************************************************************************/

//BMP390_StatusTypeDef BMP390_Init();





#endif /* INC_BMP390_H_ */
