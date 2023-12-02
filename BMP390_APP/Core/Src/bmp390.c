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

return true;

}

_Bool BMP390_Get_RawCalibCoeff(BMP390_HandleTypeDef *BMP390){

	uint8_t BMP390_CalibCoeff[21];
	uint8_t cnt = 0;

	HAL_I2C_Mem_Read(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_StartAdd_CalibCoeff, 1, &BMP390_CalibCoeff[0], 21, 1000);
	BMP390->Raw_NVM.NVM_PAR_T1 = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8)); cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_T2 = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8)); cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_T3 = (int8_t)((BMP390_CalibCoeff[cnt])); cnt+=1;
	BMP390->Raw_NVM.NVM_PAR_P1 = (int16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8)); cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_P2 = (int16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8)); cnt+=2;
	BMP390->Raw_NVM.NVM_PAR_P3 = (int16_t)((BMP390_CalibCoeff[cnt])); cnt+=1;


return true;
}

_Bool BMP390_Calc_PrcsdCalibrationCoeff(BMP390_HandleTypeDef *BMP390){



return true;
}
