#ifndef __APRS_H
#define __APRS_H

#include "main.h"
#include "usart.h"

/**
  * @brief Weather data structure definition
  */
typedef struct
{
	uint16_t wind_direction_AD;		/*!< Wind direction AD (0-4095) */
	uint8_t wind_direction_angle;	/*!< Wind direction angle (16degrees) (0-360) */					
	uint16_t wind_frequency;		/*!< Wind frequenct (1Hz) */
	float wind_speed_RT;			/*!< Real-time wind speed (0.1m/s) */
	float wind_speed_1min_ave;		/*!< The average wind speed in 1 min (0.1m/s) */
	float wind_speed_5min_max;		/*!< The max wind speed in 5 mins (0.1m/s) */
	uint16_t rainfall_RT_count;     /*!< Real-time rainfall count (0-9999, circular) */
	uint16_t rainfall_1min_count;	/*!< Rainfall count in 1 min (0-9999) */
	float rainfall_1min;			/*!< Rainfall in 1 min (0.1mm) */
	float rainfall_1hour;			/*!< Rainfall in 1 hour (0.1mm) */
	float rainfall_24hours;			/*!< Rainfall in 24 hours (0.1mm) */
	float temperature;				/*!< Temperature in Celsius degrees (0.1 C) (below 0 show -)*/
	float humidity;					/*!< Humidfity (0.1%) (0%-99%) */
	float AtmosphericPressure;		/*!< AtmosphericPressure (0.1hpa) */
} Weather_DATA_Def;

typedef enum
{
	APRS_State_Start =  0,
	APRS_State_Gather_Finish  =	1,
	APRS_State_Process = 2,
	APRS_State_Finish = 3,
	APRS_State_Error =	4,
} APRS_State;

#define char_to_Number(a)			  ((a) - 48)
#define DATA_check(a, b, c) ( ((a) <= (b)) && ((c) >= (b)) )   //a-min, b-data, c-max

#define APRS_DATA_SIZE 80

void Weather_Get(Weather_DATA_Def * Weather_data);

#endif
