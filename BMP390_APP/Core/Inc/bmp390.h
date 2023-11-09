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
extern float  BMP390_VerticalSpeed;
extern float  BMP390_gForce;
/**
 * G force = It's a thing that creates the feeling of heaviness
 * GForce  = m * a
 * gForce  = It's mechanical force that unit mass of an object is exposed to
 * a 	   = It's acceleration that is done by the world to an object
 * m       = It's the total mass of an object
 **/

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
 	 	 	 	 	 	 	 	 	 	 	* 		 mode ==> Sleep mode  [00]
 	 	 	 	 	 	 	 	 	 	 	* 		 		  Forced mode [10]
 	 	 	 	 	 	 	 	 	 	 	* 		 		  Normal mode [11]
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

	BMP390_Filter_Coef_0   =  0,   /* filter coefficient : 0 */
	BMP390_Filter_Coef_1   =  1,   /* filter coefficient : 1 */
	BMP390_Filter_Coef_3   =  2,   /* filter coefficient : 3 */
	BMP390_Filter_Coef_7   =  3,   /* filter coefficient : 7 */
	BMP390_Filter_Coef_15  =  4,   /* filter coefficient : 15 */
	BMP390_Filter_Coef_31  =  5,   /* filter coefficient : 31 */
	BMP390_Filter_Coef_63  =  6,   /* filter coefficient : 63 */
	BMP390_Filter_Coef_127 =  7    /* filter coefficient : 127 */

}BMP390_FilterCoefTypeDef;


/**
 * @brief BMP390 sensor status flag enumeration definition
 */
typedef enum{

	BMP390_Error_Fatal 		   =  1, /*Power Issues, Hardware Issues */
	BMP390_Error_Command       =  2, /*Communication & Time Issues, Abnormal Input Data Inconsistencies (!Cleared on read)*/
	BMP390_Error_Configuration =  4  /*Sensor Configuration (!Cleared on read)*/

}BMP390_ErrorTypeDef;


/**
 * @brief BMP390 pressure and temperature data status flag enumeration definition
 */
typedef enum{

	BMP390_drdy_CMD 	 = (1<<4),  /*Command decoder is ready to accept a new command*/
	BMP390_drdy_Press 	 = (1<<5),	/*Data ready for pressure*/
	BMP390_drdy_Temp  	 = (1<<6)   /*Data ready for temperature sensor*/

}BMP390_StatusTypeDef;


/**
 * @brief BMP390 oversampling enumeration definition for pressure and temperature data
 */
typedef enum{

	BMP390_Oversampling_X1 	= 0,	/*No oversampling*/
	BMP390_Oversampling_X2 	= 1,	/*x2 oversampling*/
	BMP390_Oversampling_X4 	= 2,	/*x4 oversampling*/
	BMP390_Oversampling_X8 	= 3,	/*x8 oversampling*/
	BMP390_Oversampling_X16 = 4,	/*x16 oversampling*/
	BMP390_Oversampling_X32 = 5		/*x32 oversampling*/

}BMP390_Osrs_TypeDef;


/**
 * @brief BMP390 working mode enumeration definition
 */
typedef enum{

	BMP390_Mode_Sleep  = 0,
	BMP390_Mode_Forced = 1,
	BMP390_Mode_Normal = 2

}BMP390_ModeTypeDef;


/**
 * @brief BMP390 (de)active sensor press and temp enumeration definition
 */
typedef enum{

	BMP390_Disable_TP = 0,
	BMP390_Eneable_TP = 1

}BMP390_En_TypeDef;


typedef enum{



};





/******************************************************************************/
/*!@name         	BMP390 Structures                                         */
/******************************************************************************/

/******************************************************************************/
/*!@name          	BMP390 Function Prototypes            			  		  */
/******************************************************************************/

//BMP390_StatusTypeDef BMP390_Init();





#endif /* INC_BMP390_H_ */
