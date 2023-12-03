/*!
 *  @file : bmp390.c
 *  @date : 29-10-2023
 *
 *
 *      Author: Yunus Emre KAYRA (https://github.com/YEK-Kayra)
 ******************************************************************************
 * 	@attention
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 TAISAT Turkish Artificial Intelligence Supported Autonomous Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * THE PLANS
 * Press Filter coef ==> 8
 * Oversampling press ==> x8 , Oversampling temp ==> x2
 * Odr value ==> 50Hz
 *
 * NOTE ==> if our sensor does soft reset. All params of the sensor are cleared. We have to init our sensor properly again.
 */

#include "bmp390.h"

_Bool BMP390_Init(BMP390_HandleTypeDef *BMP390){

	 if(HAL_I2C_IsDeviceReady(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, 10, 1000) != HAL_OK){

		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	 }

	 BMP390_Get_RawCalibCoeff(BMP390);

	 BMP390_Calc_PrcsdCalibrationCoeff(BMP390);

	 BMP390_Set_DefaultParams(BMP390);

	 BMP390->PWR_CTRL = ((BMP390->BMP390_Params.bmp390Mode)<<4) |
			 	 	 	((BMP390->BMP390_Params.bmp390_stat_meas_temp)<<1)|
			 	 	 	((BMP390->BMP390_Params.bmp390_stat_meas_press)<<0);


	 BMP390->CONFIG = ((BMP390->BMP390_Params.bmp390_filtercoef)<<1);
	 BMP390->ODR = (BMP390->BMP390_Params.bmp390_odr);

	 BMP390->OSR = ((BMP390->BMP390_Params.bmp390_press_osrs)<<0) |
			 	   ((BMP390->BMP390_Params.bmp390_temp_osrs)<<3) ;


return true;

}

_Bool BMP390_Get_RawCalibCoeff(BMP390_HandleTypeDef *BMP390){

	uint8_t BMP390_CalibCoeff[21];
	uint8_t cnt = 0;

	HAL_I2C_Mem_Read(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_StartAdd_CalibCoeff, 1, &BMP390_CalibCoeff[0], 21, 1000);

	BMP390->Raw_NVM.NVM_PAR_T1  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_T2  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_T3  = (int8_t)((BMP390_CalibCoeff[cnt])); 								     cnt+=1;
	BMP390->Raw_NVM.NVM_PAR_P1  = (int16_t)((BMP390_CalibCoeff[cnt])  | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_P2  = (int16_t)((BMP390_CalibCoeff[cnt])  | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_P3  = (int8_t)((BMP390_CalibCoeff[cnt])); 								     cnt+=1;
	BMP390->Raw_NVM.NVM_PAR_P4  = (int8_t)((BMP390_CalibCoeff[cnt])); 								     cnt+=1;
	BMP390->Raw_NVM.NVM_PAR_P5  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_P6  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_P7  = (int8_t)((BMP390_CalibCoeff[cnt])); 									 cnt+=1;
	BMP390->Raw_NVM.NVM_PAR_P8  = (int8_t)((BMP390_CalibCoeff[cnt]));  									 cnt+=1;
	BMP390->Raw_NVM.NVM_PAR_P9  = (int16_t)((BMP390_CalibCoeff[cnt])  | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_P10 = (int8_t)((BMP390_CalibCoeff[cnt])); 									 cnt+=1;
	BMP390->Raw_NVM.NVM_PAR_P11 = (int8_t)((BMP390_CalibCoeff[cnt]));

return true;

}

_Bool BMP390_Calc_PrcsdCalibrationCoeff(BMP390_HandleTypeDef *BMP390){

	BMP390->Prcsd_NVM.PAR_T1 = (BMP390->Raw_NVM.NVM_PAR_T1 / pow(2,-8));
	BMP390->Prcsd_NVM.PAR_T2 = (BMP390->Raw_NVM.NVM_PAR_T2 / pow(2,30));
	BMP390->Prcsd_NVM.PAR_T3 = (BMP390->Raw_NVM.NVM_PAR_T3 / pow(2,48));
	BMP390->Prcsd_NVM.PAR_P1 = ((BMP390->Raw_NVM.NVM_PAR_P1 - pow(2,14)) / pow(2,20));
	BMP390->Prcsd_NVM.PAR_P2 = ((BMP390->Raw_NVM.NVM_PAR_P2 - pow(2,14)) / pow(2,29));
	BMP390->Prcsd_NVM.PAR_P3 = (BMP390->Raw_NVM.NVM_PAR_P3 / pow(2,32));
	BMP390->Prcsd_NVM.PAR_P4 = (BMP390->Raw_NVM.NVM_PAR_P4 / pow(2,37));
	BMP390->Prcsd_NVM.PAR_P5 = (BMP390->Raw_NVM.NVM_PAR_P5 / pow(2,-3));
	BMP390->Prcsd_NVM.PAR_P6 = (BMP390->Raw_NVM.NVM_PAR_P6 / pow(2,6));
	BMP390->Prcsd_NVM.PAR_P7 = (BMP390->Raw_NVM.NVM_PAR_P7 / pow(2,8));
	BMP390->Prcsd_NVM.PAR_P8 = (BMP390->Raw_NVM.NVM_PAR_P8 / pow(2,15));
	BMP390->Prcsd_NVM.PAR_P9 = (BMP390->Raw_NVM.NVM_PAR_P9 / pow(2,48));
	BMP390->Prcsd_NVM.PAR_P10 = (BMP390->Raw_NVM.NVM_PAR_P10 / pow(2,48));
	BMP390->Prcsd_NVM.PAR_P11 = (BMP390->Raw_NVM.NVM_PAR_P11 / pow(2,65));

return true;
}

_Bool BMP390_Set_DefaultParams(BMP390_HandleTypeDef *BMP390){

	BMP390->BMP390_Params.bmp390Mode = BMP390_Mode_Normal;
	BMP390->BMP390_Params.bmp390_stat_meas_press = Enable;
	BMP390->BMP390_Params.bmp390_stat_meas_temp = Enable;
	BMP390->BMP390_Params.bmp390_press_osrs = BMP390_Oversampling_X8 ;
	BMP390->BMP390_Params.bmp390_temp_osrs= BMP390_Oversampling_X2 ;
	BMP390->BMP390_Params.bmp390_filtercoef = BMP390_Filter_Coef_3;
	BMP390->BMP390_Params.bmp390_odr = BMP390_ODR_50;

return true;
}

