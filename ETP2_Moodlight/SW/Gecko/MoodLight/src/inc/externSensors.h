/*
 * externSensors.h
 *
 *  Created on: 29.03.2020
 *      Author: lars_
 */

#ifndef ES_EXTERNSENSORS_H_
#define ES_EXTERNSENSORS_H_

uint32_t temp_index;
uint32_t const OVERTEMP;
uint32_t tempLimit[2];

void ES_Get_Sensors_Values(void);
int16_t ES_GetTemp(void);
uint16_t ES_GetIllum(void);
void ES_init(void);
void ES_TempToColor(void);
uint32_t ES_IllumToBright();
int32_t ES_GetAvgTemp(int32_t);


#endif /* EX_EXTERNSENSORS_H_ */
