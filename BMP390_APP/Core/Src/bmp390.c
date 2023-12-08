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

	 BMP390_Upload_ConfigParams(BMP390);


	 if(BMP390->Ref_Alt_Sel == 'm'){

		 BMP390->FixedAltitude = 0.0; //We set zero at the first time because gets the real place altitude value
		 BMP390->FixedAltitude = BMP390_Calc_TemporaryAltitude(BMP390, &BMP390_VertAlt);

	 }
	 else if(BMP390->Ref_Alt_Sel == 'M'){

		 BMP390->FixedAltitude = 0.0;

	 }

	 return true;

}

_Bool BMP390_Upload_ConfigParams(BMP390_HandleTypeDef *BMP390){

	 BMP390->PWR_CTRL = ((BMP390->Params.mode)<<4) |
			 	 	 	((BMP390->Params.stat_meas_temp)<<1)|
			 	 	 	((BMP390->Params.stat_meas_press)<<0);

	 BMP390->CONFIG   = ((BMP390->Params.filtercoef)<<1);

	 BMP390->ODR 	  = (BMP390->Params.odr);

     BMP390->OSR 	  = ((BMP390->Params.press_osrs)<<0) |
					    ((BMP390->Params.temp_osrs)<<3);


	 BMP390->DeltaData.alt0 = 0.0;
	 BMP390->DeltaData.spd0 = 0.0;
	 BMP390->DeltaData.acc0 = 0.0;
	 BMP390->DeltaData.cnt 	+= 1;


	 HAL_I2C_Mem_Write(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_REG_PWR_CTRL , 1, &BMP390->PWR_CTRL, 1, 1000);
	 HAL_I2C_Mem_Write(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_REG_CONFIG , 1, &BMP390->CONFIG, 1, 1000);
	 HAL_I2C_Mem_Write(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_REG_ODR , 1, &BMP390->ODR, 1, 1000);
	 HAL_I2C_Mem_Write(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_REG_OSR , 1, &BMP390->OSR, 1, 1000);

	 return true;
}

_Bool BMP390_Get_RawCalibCoeff(BMP390_HandleTypeDef *BMP390){

	uint8_t BMP390_CalibCoeff[21];
	uint8_t cnt = 0;

	HAL_I2C_Mem_Read(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_StartAdd_CalibCoeff, 1, &BMP390_CalibCoeff[0], 21, 1000);

	BMP390->Raw_NVM.T1  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.T2  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.T3  = (int8_t)((BMP390_CalibCoeff[cnt])); 								     cnt+=1;
	BMP390->Raw_NVM.P1  = (int16_t)((BMP390_CalibCoeff[cnt])  | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.P2  = (int16_t)((BMP390_CalibCoeff[cnt])  | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.P3  = (int8_t)((BMP390_CalibCoeff[cnt])); 								     cnt+=1;
	BMP390->Raw_NVM.P4  = (int8_t)((BMP390_CalibCoeff[cnt])); 								     cnt+=1;
	BMP390->Raw_NVM.P5  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.P6  = (uint16_t)((BMP390_CalibCoeff[cnt]) | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.P7  = (int8_t)((BMP390_CalibCoeff[cnt])); 									 cnt+=1;
	BMP390->Raw_NVM.P8  = (int8_t)((BMP390_CalibCoeff[cnt]));  									 cnt+=1;
	BMP390->Raw_NVM.P9  = (int16_t)((BMP390_CalibCoeff[cnt])  | (BMP390_CalibCoeff[cnt+1]<<8));  cnt+=2;
	BMP390->Raw_NVM.P10 = (int8_t)((BMP390_CalibCoeff[cnt])); 									 cnt+=1;
	BMP390->Raw_NVM.P11 = (int8_t)((BMP390_CalibCoeff[cnt]));

	return true;

}

_Bool BMP390_Calc_PrcsdCalibrationCoeff(BMP390_HandleTypeDef *BMP390){

	BMP390->Prcsd_NVM.T1 = (BMP390->Raw_NVM.T1 / pow(2,-8));
	BMP390->Prcsd_NVM.T2 = (BMP390->Raw_NVM.T2 / pow(2,30));
	BMP390->Prcsd_NVM.T3 = (BMP390->Raw_NVM.T3 / pow(2,48));
	BMP390->Prcsd_NVM.P1 = ((BMP390->Raw_NVM.P1 - pow(2,14)) / pow(2,20));
	BMP390->Prcsd_NVM.P2 = ((BMP390->Raw_NVM.P2 - pow(2,14)) / pow(2,29));
	BMP390->Prcsd_NVM.P3 = (BMP390->Raw_NVM.P3 / pow(2,32));
	BMP390->Prcsd_NVM.P4 = (BMP390->Raw_NVM.P4 / pow(2,37));
	BMP390->Prcsd_NVM.P5 = (BMP390->Raw_NVM.P5 / pow(2,-3));
	BMP390->Prcsd_NVM.P6 = (BMP390->Raw_NVM.P6 / pow(2,6));
	BMP390->Prcsd_NVM.P7 = (BMP390->Raw_NVM.P7 / pow(2,8));
	BMP390->Prcsd_NVM.P8 = (BMP390->Raw_NVM.P8 / pow(2,15));
	BMP390->Prcsd_NVM.P9 = (BMP390->Raw_NVM.P9 / pow(2,48));
	BMP390->Prcsd_NVM.P10 = (BMP390->Raw_NVM.P10 / pow(2,48));
	BMP390->Prcsd_NVM.P11 = (BMP390->Raw_NVM.P11 / pow(2,65));

	return true;
}

_Bool BMP390_Set_DefaultParams(BMP390_HandleTypeDef *BMP390){

	BMP390->Params.mode = BMP390_Mode_Normal;
	BMP390->Params.stat_meas_press = Enable;
	BMP390->Params.stat_meas_temp = Enable;
	BMP390->Params.press_osrs = BMP390_Oversampling_X8 ;
	BMP390->Params.temp_osrs= BMP390_Oversampling_X2 ;
	BMP390->Params.filtercoef = BMP390_Filter_Coef_3;
	BMP390->Params.odr = BMP390_ODR_50;

	return true;
}


_Bool BMP390_Get_SensorValues(BMP390_HandleTypeDef *BMP390, float *BMP390_Press,
							 float *BMP390_Temp,float *BMP390_VertAlt,
							 float *BMP390_VertAcc, float *BMP390_VertSpd,
							 float *BMP390_gForce){

	uint32_t rawPress;
	uint32_t rawTemp;
	uint8_t MSB_LSB_XLSB_PT[6] = {0};

	HAL_I2C_Mem_Read(BMP390->i2c, BMP390->BMP390_I2C_ADDRESS, BMP390_StartAdd_MSB_LSB_XLSB_PT, 1, &MSB_LSB_XLSB_PT[6], 6, 10000);

	rawPress = ((MSB_LSB_XLSB_PT[2])<<16)  | ((MSB_LSB_XLSB_PT[1])<<8) | ((MSB_LSB_XLSB_PT[0])<<0);
	rawTemp  = ((MSB_LSB_XLSB_PT[5])<<16)  | ((MSB_LSB_XLSB_PT[4])<<8) | ((MSB_LSB_XLSB_PT[3])<<0);


	*BMP390_Temp  	= BMP390_Calc_PrcsdTemp(BMP390,rawTemp);
	*BMP390_Press 	= BMP390_Calc_PrcsdPress(BMP390,rawPress,BMP390_Temp);
	*BMP390_VertAlt = BMP390_Calc_VertAlt(BMP390, BMP390_Press);

	*BMP390_VertSpd = BMP390_Calc_VertSpd(BMP390, BMP390_VertAlt);
	//Yeni değeri de atayalım işlemden sonra
	BMP390->DeltaData.holderAlt = *BMP390_VertAlt; // şu an alt1 alt0 olmalı ve yine alt1-alt0 yapılmalı holder kullanrak
	BMP390->DeltaData.cnt -= 1;


	//*BMP390_VertAcc = BMP390_Calc_VertAcc();
	//*BMP390_gForce	= BMP390_Calc_gForce();


	return true;
}


float BMP390_Calc_PrcsdTemp(BMP390_HandleTypeDef *BMP390, uint32_t rawTemp){

	float partial_data1;
	float partial_data2;


	partial_data1 = (float)(rawTemp - BMP390->Prcsd_NVM.T1);
	partial_data2 = (float)(partial_data1 * BMP390->Prcsd_NVM.T2);

	return (partial_data2 + (partial_data1 * partial_data1) * BMP390->Prcsd_NVM.T3);

}

float BMP390_Calc_PrcsdPress(BMP390_HandleTypeDef *BMP390, uint32_t rawPress, float *BMP390_Temp){

	float partial_data1;
	float partial_data2;
	float partial_data3;
	float partial_data4;
	float partial_out1;
	float partial_out2;


	partial_data1 = BMP390->Prcsd_NVM.P6 * (*(BMP390_Temp));
	partial_data2 = BMP390->Prcsd_NVM.P7 * ((*(BMP390_Temp)) * (*(BMP390_Temp)));
	partial_data3 = BMP390->Prcsd_NVM.P8 * ((*(BMP390_Temp)) * (*(BMP390_Temp)) * (*(BMP390_Temp)));
	partial_out1 =  BMP390->Prcsd_NVM.P5 + partial_data1 + partial_data2 + partial_data3;
	partial_data1 = BMP390->Prcsd_NVM.P2 * (*(BMP390_Temp));
	partial_data2 = BMP390->Prcsd_NVM.P3 * ((*(BMP390_Temp)) * (*(BMP390_Temp)));
	partial_data3 = BMP390->Prcsd_NVM.P4 * ((*(BMP390_Temp)) * (*(BMP390_Temp)) * (*(BMP390_Temp)));
	partial_out2 = (float)rawPress * (BMP390->Prcsd_NVM.P1 + partial_data1 + partial_data2 + partial_data3);
	partial_data1 = (float)rawPress * (float)rawPress;
	partial_data2 = BMP390->Prcsd_NVM.P9 + BMP390->Prcsd_NVM.P10 * (*(BMP390_Temp));
	partial_data3 = partial_data1 * partial_data2;
	partial_data4 = partial_data3 + ((float)rawPress * (float)rawPress * (float)rawPress) * BMP390->Prcsd_NVM.P11;

	return partial_out1 + partial_out2 + partial_data4;

}


float BMP390_Calc_VertAlt(BMP390_HandleTypeDef *BMP390, float *BMP390_Press){

	return (((SeaLevelTemp / GradientTemp)
			* (1 - pow((*BMP390_Press / SeaLevelPress),((GasCoefficient * GradientTemp)/GravityAccel))))
			- (BMP390->FixedAltitude));

}


float BMP390_Calc_TemporaryAltitude(BMP390_HandleTypeDef *BMP390, float *BMP390_VertAlt){

	 float tempAltitude = 0;
	 for(int cnt = 0 ; cnt < 20 ; cnt++){

		 BMP390_Get_SensorValues(BMP390, &BMP390_Press,
			  		  	  	  	 &BMP390_Temp, BMP390_VertAlt,
			  					 &BMP390_VertAcc, &BMP390_VertSpd,
			  					 &BMP390_gForce);

		 tempAltitude = (float)(tempAltitude + (float)((*BMP390_VertAlt) * (0.05)));

	  }
	 HAL_TIM_Base_Start_IT(&htim1);
	 return tempAltitude;

}


//Yükseklik değişimi ile hız hesabı,// V = (X1 - X0)/gerçek 1 saniye hızı verecek
float BMP390_Calc_VertSpd(BMP390_HandleTypeDef *BMP390, float *BMP390_VertAlt){

	if(BMP390->DeltaData.cnt == 0){

		BMP390->DeltaData.alt0 = (*BMP390_VertAlt);
		BMP390->DeltaData.cnt += 1;

	}
	else if(BMP390->DeltaData.cnt == 1){

		BMP390->DeltaData.alt1 = (*BMP390_VertAlt);

		return ((BMP390->DeltaData.alt1) - (BMP390->DeltaData.alt0));

	}

}

float BMP390_Calc_VertAcc(BMP390_HandleTypeDef *BMP390, float *BMP390_VertSpd){

}

float BMP390_Calc_gForce(BMP390_HandleTypeDef *BMP390,  float *BMP390_gForce, float *TotalMass){

}

