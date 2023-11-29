/*!
 * @file : bmp390.h
 *bu dosya şimdi mainde mi branch1 demi
 * Author: Yunus Emre KAYRA (github.com/YEK-Kayra)
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 TAISAT Turkish Artificial Intelligence Supported Autonomous Technologies
 *
 **/

// Bekle beni CEM KARACA yazıyoruz kütüphaneyi biiznillah

/*
 * $WİLL BE ADDED FEATURES$
 *
 * interrupt.h and .c files will be added
 * fifo.h and .c files will be added
 * fifo_flush register will be used , if you use soft reset , you don't forget to init the sensor again
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

#define BMP390_REG_ERR		 		0x02  /*Sensor Error conditions ==> bits: conf_err[2:2], cmd_err[1:1], fatal_err[0:0] */
#define BMP390_REG_STATUS    		0x03  /*Sensor Status Flags ==> bits: drdy_temp[6:6], drdy_press[5:5], cmd_rdy[4:4] (drdy:Data ready x)*/

#define BMP390_REG_DATA_0    		0x04  /*24Bit pressure data of XLSB byte*/
#define BMP390_REG_DATA_1    		0x05  /*24Bit pressure data of LSB 	byte*/
#define BMP390_REG_DATA_2    		0x06  /*24Bit pressure data of MSB 	byte*/
#define BMP390_REG_DATA_3    		0x07  /*24Bit temperature data of XLSB byte*/
#define BMP390_REG_DATA_4    		0x08  /*24Bit temperature data of LSB byte*/
#define BMP390_REG_DATA_5   		0x09  /*24Bit temperature data of MSB byte*/

#define BMP390_REG_PWR_CTRL   		0x1B  /**Enables or disables pressure and temperature measurement.
 	 	 	 	 	 	 	 	 	 	 	* bits : mode[5:4],temp_en[1:1], press_en[0:0]
 	 	 	 	 	 	 	 	 	 	 	*
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

#define BMP390_REG_FIFO_CONFIG_1	0x17 /**
 	 	 	 	 	 	 	 	 	 	  * fifo_mode[0:0],fifo_stop_on_full[1:1],fifo_time_en[2:2],fifo_press_en[3:3],fifo_temp_en[4:4]
 	 	 	 	 	 	 	 	 	 	  */


#define BMP390_REG_FIFO_CONFIG_2	0x18 /**
 	 	 	 	 	 	 	 	 	 	  * fifo_subsampling[2:0], data_select[3:4]
 	 	 	 	 	 	 	 	 	 	  */




/******************************************************************************/
/*!@name         		    BMP390 ENUM                                       */
/******************************************************************************/

/*******************GENERAL SENSOR FEATURES ENUMS**********************/

/**
 * @brief Work modes
 */
typedef enum{

	BMP390_Mode_Sleep  = 0,
	BMP390_Mode_Forced = 1,
	BMP390_Mode_Normal = 2

}BMP390_Mode_TypeDef;


/**
 * @brief (De)Active sensor pressure and temperature
 */
typedef enum{

	BMP390_Disable_Measurement = 0,
	BMP390_Eneable_Measurement = 1

}BMP390_Stat_Meas_TypeDef;

typedef enum{

	BMP390_SPI4W_Mode = 0,
	BMP390_SPI3W_Mode = 1

}BMP390_SPI_X_TypeDef;

typedef enum{

	BMP390_I2C_Wdt_Disable = 0,
	BMP390_I2C_Wdt_Enable  = 1

}BMP390_I2C_Wdt_En_TypeDef;

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


/**
 * @brief Store temperature data in FIFO
 */
typedef enum{

	BMP390_Fifo_Temp_Disable = 0,
	BMP390_Fifo_Temp_Enable  = 1

}BMP390_Fifo_Temp_En_TypeDef;


/**
 * @brief Store pressure data in FIFO
 */
typedef enum{

	BMP390_Fifo_Press_Disable = 0,
	BMP390_Fifo_Press_Enable  = 1

}BMP390_Fifo_Press_En_TypeDef;


/**
 * @brief Return sensortime frame after the last valid data frame
 */
typedef enum{

	BMP390_Fifo_Time_Disable = 0,
	BMP390_Fifo_Time_Enable  = 1

}BMP390_Fifo_Time_En_TypeDef;


/**
 * @brief Stop writing samples into FIFO when FIFO is full.
 */
typedef enum{

	BMP390_Fifo_StopOnFull_Disable = 0,
	BMP390_Fifo_StopOnFull_Enable  = 1

}BMP390_Fifo_StopOnFull_TypeDef;


/**
 * @brief Enables or disables the FIFO
 */
typedef enum{

	BMP390_Fifo_Mode_Disable = 0,
	BMP390_Fifo_Mode_Enable  = 1

}BMP390_Fifo_Mode_TypeDef;


/*******************STATUS AND INTERRUPT FLAG ENUMS**********************/

typedef enum{
	BMP390_Int_Out_PP = 0,
	BMP390_Int_Out_OD = 1
}BMP390_Int_Out_TypeDef;

typedef enum{
	BMP390_Int_Level_A_L = 0,
	BMP390_Int_Level_A_H = 1
}BMP390_Int_Level_TypeDef;

typedef enum{
	BMP390_Int_Latch_Disable = 0,
	BMP390_Int_Latch_Enable = 1
}BMP390_Int_Latch_TypeDef;

typedef enum{
	BMP390_Int_Fwtm_Disable = 0,
	BMP390_Int_Fwtm_Enable = 1
}BMP390_Int_Fwtm_En_TypeDef;

typedef enum{
	BMP390_Int_Ffull_Disable = 0,
	BMP390_Int_Ffull_Enable = 1
}BMP390_Int_Ffull_En_TypeDef;

typedef enum{
	BMP390_Int_Ds_L = 0,
	BMP390_Int_Ds_H = 1
}BMP390_Int_Ds_TypeDef;

typedef enum{
	BMP390_Int_Drdy_Disable = 0,
	BMP390_Int_Drdy_Enable = 1
}BMP390_Int_Drdy_En_TypeDef;

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

}BMP390_ProcessedCalibData_TypeDef;

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
	BMP390_Stat_Meas_TypeDef bmp390_meas_stat_press;
	BMP390_Stat_Meas_TypeDef bmp390_meas_stat_temp;

	/**
	 * These three of them are about Interface Configuration register (IF_CONF), Variables of IF_Config variable
	 */
	BMP390_SPI_X_TypeDef bmp390_spi_x;
	BMP390_I2C_Wdt_En_TypeDef bmp390_wdt_stat;
	BMP390_I2C_Wdt_Sel_TypeDef bmp390_wdt_type;

	/**
	 * These five of them are about FIFO Configuration 1 register (“FIFO_CONFIG_1), Variables of FIFO_CONFIG_1 variable
	 */
	BMP390_Fifo_Mode_TypeDef bmp390_fifo_mode;
	BMP390_Fifo_StopOnFull_TypeDef bmp390_fifo_stopFull;
	BMP390_Fifo_Press_En_TypeDef bmp390_fifo_press_stat;
	BMP390_Fifo_Temp_En_TypeDef bmp390_fifo_temp_stat;
	BMP390_Fifo_Time_En_TypeDef bmp390_fifo_time_stat;

	/**
	 * These two of them are about FIFO Configuration 2 register (“FIFO_CONFIG_2), Variables of FIFO_CONFIG_2 variable
	 */
	BMP390_Fifo_Subsampling_TypeDef bmp390_fifo_subs;
	BMP390_Fifo_DataSelect_TypeDef bmp390_fifo_sel;

	//Bu kısım ve diğer kısımlardaki bulunan eneable disable opsiyonlular tek bir enum ve struct altında toplanması üzerine mimari çıkartılcak
	BMP390_Int_Out_TypeDef bmp390_ınt_out;
	BMP390_Int_Level_TypeDef bmp390_ınt_level;
	BMP390_Int_Latch_TypeDef bmp390_ınt_latch;
	BMP390_Int_Fwtm_En_TypeDef bmp390_ınt_fwtm;
	BMP390_Int_Ffull_En_TypeDef bmp390_ınt_fful;
	BMP390_Int_Ds_TypeDef bmp390_ınt_ds;
	BMP390_Int_Drdy_En_TypeDef bmp390_ınt_drdy;

}BMP390_Params_t;


typedef struct{

	BMP390_Params_t BMP390_Params; //Bir öge oluştur==>Mode,ODR,OSRS,FİLTER,interface,sıcaklıkbasınçeneable disable

	uint16_t BMP280_I2C_ADDRESS;   //Yazılan i2c adresinin genel halini tutsun dinamik bir değişken

	I2C_HandleTypeDef *i2c;			//i2c handle i gerektiği yerde buradan veri çekeceğiz

	BMP390_RawCalibData_TypeDef Raw_NVM; // Raw kalibrasyon verilerine ulaşacağımız yer

	BMP390_ProcessedCalibData_TypeDef Prcd_NVM; // İşlenmiş verileri kayıt edip kullanacağımız yer

	uint8_t IF_CONFIG; // paramslarla değer belirlencek
	uint8_t PWR_CTRL;  // paramslarla değer belirlencek
	uint8_t OSRS; 	   //  paramslarla değer belirlencek
	uint8_t FIFO_CONFIG_1; //  paramslarla değer belirlencek
	uint8_t FIFO_CONFIG_2; //  paramslarla değer belirlencek
	uint8_t INT_CTRL; //  paramslarla değer belirlencek


}BMP390_HandleTypeDef;


/******************************************************************************/
/*!@name          	BMP390 Function Prototypes            			  		  */
/******************************************************************************/

void BMP390_Init();

void BMP390_Config_CalibDatas();

void BMP390_Set_DefaultParams();



#endif /* INC_BMP390_H_ */
