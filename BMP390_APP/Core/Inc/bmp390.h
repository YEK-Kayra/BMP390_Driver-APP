/*!
 * @file : bmp390.h

 * Author: Yunus Emre KAYRA (github.com/YEK-Kayra)
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 TAISAT Turkish Artificial Intelligence Supported Autonomous Technologies
 *
 **/

#ifndef INC_BMP390_H_
#define INC_BMP390_H_


/******************************************************************************/
/*!@name         Header includes            				    			  */
/******************************************************************************/
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

/******************************************************************************/
/*!@name          BMP390 Registers   			    		    			  */
/******************************************************************************/


#define BMP390_REG_CONFIG		 	0x1F  /*Controls the IIR filter coefficients.
 	 	 	 	 	 	 	 	 	 	    bits :  iir_filter[3:1] */

#define BMP390_REG_ODR   			0x1D  /*Set the configuration of the output data rates
 	 	 	 	 	 	 	 	 	 	 	bits :  odr_sel[4:0] */

#define BMP390_REG_OSR   			0x1C  /*Controls the oversampling settings for pressure and temperature measurements.
 	 	 	 	 	 	 	 	 	 	 	bits :  osr_t[5:3](for temperature), osr_p[2:0](for pressure)*/

#define BMP390_REG_PWR_CTRL   		0x1B  /*Enables or disables pressure and temperature measurement.
 	 	 	 	 	 	 	 	 	 	    bits : mode[5:4],temp_en[1:1], press_en[0:0] */

#define BMP390_REG_IF_CONF   		0x1A  /*controls the serial interface settings.
 	 	 	 	 	 	 	 	 	 	    bits : i2c_wdt_sel[2:2],l i2c_wdt_en[1:1], spi3[0:0] */

#define BMP390_REG_INT_CTRL   		0x19  /*Interrupt configuration can be set in the “INT_CTRL” register.
									  		bits : drdy_en[6:6], int_ds[5:5],
									  			   ffull_en[4:4], fwtm_en[3:3],
									 			   int_latch[2:2], int_level[1:1],
									 			   int_od[0:0] */

#define BMP390_REG_FIFO_CONFIG_1	0x17  /*FIFO frame content configuration.
 	 	 	 	 	 	 	 	 	 	    bits : fifo_mode[0:0],fifo_stop_on_full[1:1],fifo_time_en[2:2],fifo_press_en[3:3],fifo_temp_en[4:4]*/

#define BMP390_REG_FIFO_CONFIG_2	0x18  /*extends the FIFO_CONFIG_1 register
 	 	 	 	 	 	 	 	 	 	    bits : fifo_subsampling[2:0], data_select[3:4] */

#define BMP390_REG_FIFO_WTM_0_1		0x15  /*The FIFO byte counter indicates the current fill level of the FIFO buffer
										    16Bit fifo watermark MSB[15:8],LSB[7:0]
										    bytes {FIFO_WTM_1(0x16),FIFO_WTM_0(0x15)} */

#define BMP390_REG_FIFO_DATA		0x14  /*The data output register
										    bits : fifo_subsampling[2:0], data_select[3:4]*/

#define BMP390_REG_FIFO_LENGTH_0_1	0x12   /*The FIFO byte counter indicates the current fill level of the FIFO buffer
											 16Bit fifo lenght MSB[15:8],LSB[7:0]
											 bytes {FIFO_LENGTH_1(0x13),FIFO_LENGTH_0(0x12)} */

#define BMP390_REG_INT_STATUS		0x11  /*Shows interrupt status and is cleared after reading.
										    bits : drdy[3:3], ffull_int[1:1], fwm_int[0:0]*/

#define BMP390_REG_EVENT		    0x10  /*Contains the sensor status flags.
										    bits : itf_act_pt[1:1], por_detected[0:0]*/

#define BMP390_REG_SENSORTIME_0_3   0x0C  /*24Bit pressure MSB[23:16],LSB[15:8],XLSB[7:0]
											bytes {SENSORTIME_2(0x0E),SENSORTIME_1(0x0D),SENSORTIME_0(0x0C)} */

#define BMP390_REG_DATA_0_5    		0x04  /*24Bit pressure MSB[23:16],LSB[15:8],XLSB[7:0]
											bytes {DATA2(0x06),DATA1(0x05),DATA0(0x04)}
											24Bit temperature MSB[23:16],LSB[15:8],XLSB[7:0]
											bytes {DATA5(0x09),DATA4(0x08),DATA3(0x07)} */

#define BMP390_REG_STATUS    		0x03  /*Sensor Status Flags,(drdy:Data ready) bits: drdy_temp[6:6],
											drdy_press[5:5], cmd_rdy[4:4] */

#define BMP390_REG_ERR		 		0x02  /*Sensor Error conditions, bits: conf_err[2:2], cmd_err[1:1], fatal_err[0:0] */



/******************************************************************************/
/*!@name         		    BMP390 ENUM                                       */
/******************************************************************************/

/*******************GENERAL SENSOR FEATURES ENUMS**********************/

typedef enum{

	Disable = 0,
	Enable  = 1

}BMP390_REG_Stat_bits_TypeDef;

/**
 * @brief Work modes
 */
typedef enum{

	BMP390_Mode_Sleep  = 0,
	BMP390_Mode_Forced = 1,
	BMP390_Mode_Normal = 2

}BMP390_Mode_TypeDef;


typedef enum{

	BMP390_SPI4W_Mode = 0,
	BMP390_SPI3W_Mode = 1

}BMP390_SPI_X_TypeDef;


typedef enum{

	BMP390_I2C_Selt_Short = 0,
	BMP390_I2C_Sel_Long  = 1

}BMP390_I2C_Wdt_Sel_TypeDef;
/**
 * @brief CMD register will operate softreset and flushing FIFO
 */
typedef enum{

	BMP390_CMD_Fifoflush = 0xB0, /*Clears all data in the FIFO, does not change FIFO_CONFIG registers*/
	BMP390_CMD_Softreset = 0xB6  /*Triggers a reset, all user configuration settings are overwritten with their default state*/

}BMP390_CMD_TypeDef;


/**
 * @brief IIR filter coefficients
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

}BMP390_FilterCoef_TypeDef;


/**
 * @brief Oversampling enumeration definition for pressure and temperature data
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
 * @brief Output data rates by means of setting the (subdivision/subsampling)
 */
typedef enum{

	BMP390_ODR_200	 = 0,	/*Sampling period = 5ms*/
	BMP390_ODR_100 	 = 1,	/*Sampling period = 10ms*/
	BMP390_ODR_50    = 2,	/*Sampling period = 20ms*/
	BMP390_ODR_25  	 = 3,	/*Sampling period = 40ms*/
	BMP390_ODR_12p5  = 4,	/*Sampling period = 80ms*/
	BMP390_ODR_6p25  = 5,	/*Sampling period = 160ms*/
	BMP390_ODR_3p1   = 6,	/*Sampling period = 320ms*/
	BMP390_ODR_1p5   = 7,	/*Sampling period = 640ms*/
	BMP390_ODR_0p78  = 8,	/*Sampling period = 1.280s*/
	BMP390_ODR_0p39  = 9,	/*Sampling period = 2.560ms*/
	BMP390_ODR_0p2   = 10,	/*Sampling period = 5.120s*/
	BMP390_ODR_0p1   = 11,	/*Sampling period = 10.24s*/
	BMP390_ODR_0p05  = 12,	/*Sampling period = 20.48s*/
	BMP390_ODR_0p02  = 13,	/*Sampling period = 40.96s*/
	BMP390_ODR_0p01  = 14,	/*Sampling period = 81.92s*/
	BMP390_ODR_0p006 = 15,	/*Sampling period = 163.84s*/
	BMP390_ODR_0p003 = 16,	/*Sampling period = 327.68s*/
	BMP390_ODR_0p0015 = 17, /*Sampling period = 655.36s*/

}BMP390_ODR_TypeDef;



/***********************FIFO FEATURES ENUMS**************************/

/**
 * @brief FIFO downsampling selection for pressure and temperature data, factor is 2^fifo_subsampling
 */
typedef enum{

	BMP390_FifoSub_0 = 0,
	BMP390_FifoSub_1 = 1,
	BMP390_FifoSub_2 = 2,
	BMP390_FifoSub_3 = 3,
	BMP390_FifoSub_4 = 4,
	BMP390_FifoSub_5 = 5,
	BMP390_FifoSub_6 = 6,
	BMP390_FifoSub_7 = 7,

}BMP390_Fifo_Subsampling_TypeDef;


/**
 * @brief FIFO data source for pressure and temperature
 */
typedef enum{

	BMP390_Fifo_UnfilteredData  = 0, /*Unfiltered data (compensated or uncompensated) */
	BMP390_Fifo_FilteredData 	= 1	 /*Filtered data (compensated or uncompensated)   */

}BMP390_Fifo_DataSelect_TypeDef;


/*******************STATUS AND INTERRUPT FLAG ENUMS**********************/

/**
 * @brief
 */
typedef enum{
	BMP390_Int_Out_PP = 0,
	BMP390_Int_Out_OD = 1
}BMP390_Int_Out_TypeDef;


/**
 * @brief
 */
typedef enum{
	BMP390_Int_Level_A_L = 0,
	BMP390_Int_Level_A_H = 1
}BMP390_Int_Level_TypeDef;


/**
 * @brief
 */
typedef enum{
	BMP390_Int_Ds_L = 0,
	BMP390_Int_Ds_H = 1
}BMP390_Int_Ds_TypeDef;


/**
 * @brief Event status. These variable for check the register's flags
 */
typedef enum{

	BMP390_Event_por_detected = 1,
	BMP390_Event_itf_act_pt   = 1

}BMP390_Event_TypeDef;


/**
 * @brief Interrupt status
 */
typedef enum{

	BMP390_IntStat_Fifo_wm 	 = (1<<0),
	BMP390_IntStat_Fifo_full = (1<<0),
	BMP390_IntStat_drdy		 = (1<<0)

}BMP390_IntStat_TypeDef;


/**
 * @brief Pressure and temperature data status flag
 */
typedef enum{

	BMP390_drdy_CMD 	 = (1<<4),  /*Command decoder is ready to accept a new command*/
	BMP390_drdy_Press 	 = (1<<5),	/*Data ready for pressure*/
	BMP390_drdy_Temp  	 = (1<<6)   /*Data ready for temperature sensor*/

}BMP390_DataStatus_TypeDef;


/**
 * @brief Sensor status flag
 */
typedef enum{

	BMP390_Error_Fatal 		   =  1, /*Power Issues, Hardware Issues */
	BMP390_Error_Command       =  2, /*Communication & Time Issues, Abnormal Input Data Inconsistencies (!Cleared on read)*/
	BMP390_Error_Configuration =  4  /*Sensor Configuration (!Cleared on read)*/

}BMP390_Error_TypeDef;


/******************************************************************************/
/*!@name         	BMP390 Structures                                         */
/******************************************************************************/

typedef struct{

	/**
	 * PAR_T/P parameters are processed data. We use them when calculating Pressure, Temperature compensation
	 */
	float PAR_T1;
	float PAR_T2;
	float PAR_T3;
	float PAR_P1;
	float PAR_P2;
	float PAR_P3;
	float PAR_P4;
	float PAR_P5;
	float PAR_P6;
	float PAR_P7;
	float PAR_P8;
	float PAR_P9;
	float PAR_P10;
	float PAR_P11;

}BMP390_PrcsdCalibData_TypeDef;

typedef struct{

	/**
	 * NVM_PAR_T/P parameters are raw datas. We have to convert them to PAR_T/P params by using datasheet
	 */
	uint16_t NVM_PAR_T1;
	uint16_t NVM_PAR_T2;
	int16_t NVM_PAR_T3;
	int16_t NVM_PAR_P1;
	int16_t NVM_PAR_P2;
	int16_t NVM_PAR_P3;
	int16_t NVM_PAR_P4;
	uint16_t NVM_PAR_P5;
	uint16_t NVM_PAR_P6;
	int16_t NVM_PAR_P7;
	int16_t NVM_PAR_P8;
	int16_t NVM_PAR_P9;
	int16_t NVM_PAR_P10;
	int16_t NVM_PAR_P11;

}BMP390_RawCalibData_TypeDef;

//int_ctrl için

typedef struct { //registerlara kayıt için toplican bu parametreleri

	BMP390_Mode_TypeDef bmp390Mode;
	BMP390_ODR_TypeDef	bmp390_odr;
	BMP390_FilterCoef_TypeDef bmp390_filtercoef;

	/**
	 * These two of them are about oversampling settings register (OSR), Variables of PWR_CTRL variable
	 */
	BMP390_Osrs_TypeDef bmp390_press_osrs;
	BMP390_Osrs_TypeDef bmp390_temp_osrs;

	/**
	 * These two of them are about eneable disable measurement , Variables of PWR_CTRL variable
	 */
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_meas_press;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_meas_temp;

	/**
	 * These three of them are about Interface Configuration register (IF_CONF)
	 */
	BMP390_SPI_X_TypeDef bmp390_spi_x;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_wdt;
	BMP390_I2C_Wdt_Sel_TypeDef bmp390_wdt_type;

	/**
	 * These five of them are about FIFO Configuration_1 register (FIFO_CONFIG_1)
	 */
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_fifo;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_fifo_stopFull;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_fifo_press;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_fifo_temp;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_fifo_time;

	/**
	 * These two of them are about FIFO Configuration_2 register (FIFO_CONFIG_2)
	 */
	BMP390_Fifo_Subsampling_TypeDef bmp390_fifo_subs;
	BMP390_Fifo_DataSelect_TypeDef bmp390_fifo_sel;

	/**
	 * These seven of them are about Interrupt configuration register (INT_CTRL)
	 */
	BMP390_Int_Out_TypeDef bmp390_ınt_out;
	BMP390_Int_Level_TypeDef bmp390_ınt_level;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_int_latch;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_int_fwtm;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_int_fful;
	BMP390_Int_Ds_TypeDef bmp390_int_ds;
	BMP390_REG_Stat_bits_TypeDef bmp390_stat_int_drdy;

}BMP390_Params_t;


typedef struct{

	BMP390_Params_t BMP390_Params;

	uint16_t BMP280_I2C_ADDRESS;

	I2C_HandleTypeDef *i2c;

	BMP390_RawCalibData_TypeDef Raw_NVM;

	BMP390_PrcsdCalibData_TypeDef Prcd_NVM;

	uint8_t CONFIG; 				/*! bits: iir_filter[3:1] */
	uint8_t ODR;					/*! bits: odr_sel[4:0] */
	uint8_t OSR;					/*! bits: osr_t[5:3], osr_p[2:0] */
	uint8_t PWR_CTRL;				/*! bits: mode[5:4], temp_en[1:1], press_en[0:0] */
	uint8_t IF_CONFIG;				/*! bits: i2c_wdt_sel[2:2], i2c_wdt_en[1:1], spi3[0:0] */

	uint8_t INT_CTRL;				/**!bits:
									 * 		drdy_en[6:6], int_ds[5:5],
									 * 		ffull_en[4:4], fwtm_en[3:3],
									 * 		int_latch[2:2], int_level[1:1],
									 * 		int_od[0:0]
									 */

	uint8_t FIFO_CONFIG_1;			/**!bits:
	 	 	 	 	 	 	 	 	 * 		fifo_temp_en[4:4], fifo_press_en[3:3],
									 * 		fifo_time_en[2:2], fifo_stop_on_full[1:1],
									 * 		fifo_mode[0:0]
									 */


	uint8_t FIFO_CONFIG_2;			/*! bits: data_select[4:3], fifo_subsampling[2:0] */

	uint8_t FIFO_WTM_0;				/*! bits: fifo_water_mark_0_7[7:0] */
	uint8_t FIFO_WTM_1;				/*! bits: fifo_water_mark_8[0:0] */





}BMP390_HandleTypeDef;


/******************************************************************************/
/*!@name          	BMP390 Function Prototypes            			  		  */
/******************************************************************************/

void BMP390_Init();

void BMP390_Config_CalibDatas();

void BMP390_Set_DefaultParams();



#endif /* INC_BMP390_H_ */
