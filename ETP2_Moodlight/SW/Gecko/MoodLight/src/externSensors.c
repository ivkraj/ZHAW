/** ***************************************************************************
 * @file
 * @brief Global variables, defines and functions
 *
 * Prefix: ES
 *
 * @author Hanspeter Hochreutener (hhrt@zhaw.ch)
 * @author Lars Mueggler (muegglar@students.zhaw.ch)
 * @author Ivan Krajinovic (krajiiva@students.zhaw.ch)
 * @date 29.03.2020
 * @version 1.0.0
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

// System files
#include "em_chip.h"
#include "em_device.h"
#include "em_system.h"
#include "em_cmu.h"
#include "em_emu.h"

// Own written files
#include "globals.h"
#include "pwm.h"
#include "i2c_com.h"
#include "communication.h"
#include "userinterface.h"
#include "gpioInterrupt.h"

/******************************************************************************
 * Constants
 *****************************************************************************/
#define TEMP_ADDR				0x48
#define ILLUM_ADDR				0x44
#define UPPERILLUMRESOLUTION	500
#define UNDERILLUMLIMIT			10

/******************************************************************************
 * Variables
 *****************************************************************************/
uint8_t tempBuffer[2];
uint8_t illumBuffer[2];
uint8_t tempReg[1] = {0x00};
uint8_t illumResultReg[1] = {0x00};
uint8_t illumConfigReg[1] = {0x01};
uint8_t illumConfig[3]= {0x01, 0xCE, 0x00};
uint32_t temp_index = 0;
int32_t const OVERTEMP = 50, HYSTERESIS = 5, OFFSETTEMP = 32;
int32_t tempLimit[2] = {160, 240};
/******************************************************************************
 * Functions
 *****************************************************************************/

int32_t ES_GetTemp(void) {
	I2C_ReceiveData(TEMP_ADDR, 2, tempBuffer);
	uint16_t temp = ((((uint16_t) tempBuffer[0]) << 3) | (((uint16_t) tempBuffer[1]) >> 5));
	int32_t resultTemp;

	if (tempBuffer[0] & 0x80){
		resultTemp = temp - (2 << 11);
	} else{
		resultTemp = (int32_t) temp;
	}
	return resultTemp - OFFSETTEMP;      // to get actual Temperature -> temp/8
}

int32_t ES_GetAvgTemp(int32_t temp[4]) {
	int i = 0;
	int32_t sumTemp = 0;
	for(i = 0; i < 4; i++){
		sumTemp += temp[i];
	}
	return sumTemp/4;
}


void TempSens_RegisterSet() {
	I2C_WriteData(TEMP_ADDR, 1, tempReg);
}

void IllumSens_RegisterSet () {
	// Write to set configuration register
	I2C_WriteData(ILLUM_ADDR, 3, illumConfig);
	// Write to receive result register
	I2C_WriteData(ILLUM_ADDR, 1, illumResultReg);
}

uint16_t ES_GetIllum(void) {
	I2C_ReceiveData(ILLUM_ADDR, 2, illumBuffer);
	uint16_t illum_exp = (illumBuffer[0] & 0xF0) >> 4;
	uint16_t illum_reg = ((uint16_t)(illumBuffer[0] & 0x0F) << 8) | illumBuffer[1];
	uint16_t illum_val = ((1 << illum_exp) * illum_reg)/100;
	return illum_val;
}

void ES_init(void){
	TempSens_RegisterSet();
	IllumSens_RegisterSet();
}

void ES_TempToColor (void){
	uint32_t blue_value, red_value;
	uint32_t FadeResolution = (4096 * 255) / (tempLimit[1] - tempLimit[0]);

	if(temperature <= tempLimit[0]){
		blue_value = 255;
		red_value = 0;
	} else if(temperature >= tempLimit[1]){
		blue_value = 0;
		red_value = 255;
	} else{
		red_value = ((temperature - tempLimit[0]) * FadeResolution) / 4096;
		blue_value = ((tempLimit[1] - temperature) * FadeResolution) / 4096;
	}

	PWM_TIMER0_RED_change(red_value);
	PWM_TIMER0_BLUE_change(blue_value);
	PWM_LETIMER0_WHITE_change(0);
	PWM_TIMER0_GREEN_change(0);

}

uint32_t getIllumUpperlimit(uint32_t resolutionNumber){
	return UPPERILLUMRESOLUTION * resolutionNumber;
}

uint32_t ES_IllumToBright(void){
	uint32_t sens_value;
	uint32_t FadeResolution = (4096 * 255) / (getIllumUpperlimit(illumination_settings[2]) - UNDERILLUMLIMIT);

	if(illumination_settings[1] == 1){
		if(illumination <= UNDERILLUMLIMIT){
			sens_value = 0;
		} else if(illumination >= getIllumUpperlimit(illumination_settings[2])){
			sens_value = MAX_VALUE;
		} else{
			sens_value = (illumination * FadeResolution) / 4096;
		}

		if(illumination_settings[0] == 1){
			sens_value = MAX_VALUE-sens_value;
		}
	} else{
		sens_value = MAX_VALUE;
	}
	return sens_value;

}

/** ***************************************************************************
 * @brief Start crystal oscillators and use these clock sources
 *
 * High frequency clock = 32MHz (defined by crystal on the starter kit)
 * Low frequency clock = 32.768kHz (defined by crystal on the starter kit)
 *****************************************************************************/
void ES_Get_Sensors_Values(void){
	if((illumination_settings[1] == 1) && (newIllumFlag == true)){
		illumination = ES_GetIllum();
		newIllumFlag = false;
		if(GPIOgetBTState()){
			UI_value_changed = true;
		}
	}

	if(measTempFlag == true){
		temp_values[temp_index] = ES_GetTemp();
		measTempFlag = false;
	}

	if(newTempFlag == true){
		temperature = ES_GetAvgTemp(temp_values);

		if((temperature >= 8*OVERTEMP) && (overTempFlag == false)){
			changeStateTo(RGBW);
			RGBW_Color[0] = 0;
			RGBW_Color[1] = 0;
			RGBW_Color[2] = 0;
			RGBW_Color[3] = 0;
			if(COM_TX_Busy() == false){
				COM_TX_PutData(overTempONString, 16);
				overTempFlag = true;
			}
		}
		if((temperature <= 8*(OVERTEMP-HYSTERESIS)) && (overTempFlag == true)){
			if(COM_TX_Busy() == false){
				COM_TX_PutData(overTempOFFString, 16);
				overTempFlag = false;
			}
		}
		convertTemperatureToString(temperature);
		if((COM_TX_Busy() == false) && (GPIOgetBTState())){
			COM_TX_PutData(temperatureString, 16);
		}

		newTempFlag = false;

		if(GPIOgetBTState()){
			UI_value_changed = true;
		}
	}
}
