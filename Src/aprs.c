#include "aprs.h"
#include "Printf_in_UART.h"

static uint8_t APRS_DATA[APRS_DATA_SIZE];

static APRS_State aprs_state = APRS_State_Start;

static void APRS_DATA_Receive(uint8_t * aprs_data);
static void APRS_DATA_Process(Weather_DATA_Def * Weather_data, uint8_t * aprs_data);

void Weather_Get(Weather_DATA_Def * Weather_data)
{
	aprs_state = APRS_State_Start;
	APRS_DATA_Receive(APRS_DATA);
	APRS_DATA_Process(Weather_data, APRS_DATA);
	
	while (aprs_state != APRS_State_Finish)
	{
		if (aprs_state == APRS_State_Error)
		{
			//printf("error!\n");
			aprs_state = APRS_State_Start;
			APRS_DATA_Receive(APRS_DATA);
			APRS_DATA_Process(Weather_data, APRS_DATA);
		}
	}
}

static void APRS_DATA_Receive(uint8_t * aprs_data)
{
	HAL_UART_Receive(&huart2, aprs_data, APRS_DATA_SIZE, 1000);
	aprs_state = APRS_State_Gather_Finish;
	//printf("* %s *", aprs_data);
}

static void APRS_DATA_Process(Weather_DATA_Def * Weather_data, uint8_t * aprs_data)
{
	uint8_t i, temp;
	double temp_data;
	
	if (aprs_state == APRS_State_Gather_Finish)
	{
		aprs_state = APRS_State_Process;
		
		for (i = 0; i < APRS_DATA_SIZE; i++)
		{
			temp = aprs_data[i];
			
			if (temp == 'A')
			{
				temp_data =  (char_to_Number(aprs_data[i + 1]) * 1000) + (char_to_Number(aprs_data[i + 2]) * 100) + (char_to_Number(aprs_data[i + 3]) * 10) + (char_to_Number(aprs_data[i + 4]));
				
				if (DATA_check(0, temp_data, 4095))
				{
					Weather_data->wind_direction_AD = (uint16_t)temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
				
			}
			else if (temp == 'B')
			{
				temp_data =  char_to_Number(aprs_data[i + 1]) * 100 + char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]);
				
				if (DATA_check(0, temp_data, 365))
				{
					Weather_data->wind_direction_angle = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'C')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 1000 + char_to_Number(aprs_data[i + 2]) * 100 + char_to_Number(aprs_data[i + 3]) * 10 + char_to_Number(aprs_data[i + 4]);
			
				if (DATA_check(0, temp_data, 1000))
				{
					Weather_data->wind_frequency = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'D')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 100 + char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
						
				if (DATA_check(0, temp_data, 100))
				{
					Weather_data->wind_speed_RT = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'E')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 100 + char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
									
				if (DATA_check(0, temp_data, 100))
				{
					Weather_data->wind_speed_1min_ave  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'F')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 100 + char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
												
				if (DATA_check(0, temp_data, 100))
				{
					Weather_data->wind_speed_5min_max  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'G')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 1000 + char_to_Number(aprs_data[i + 2]) * 100 + char_to_Number(aprs_data[i + 3]) * 10 + char_to_Number(aprs_data[i + 4]);
															
				if (DATA_check(0, temp_data, 9999))
				{
					Weather_data->rainfall_RT_count  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'H')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 1000 + char_to_Number(aprs_data[i + 2]) * 100 + char_to_Number(aprs_data[i + 3]) * 10 + char_to_Number(aprs_data[i + 4]);
																		
				if (DATA_check(0, temp_data, 9999))
				{
					Weather_data->rainfall_1min_count  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'I')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 100 + char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
																					
				if (DATA_check(0, temp_data, 100))
				{
					Weather_data->rainfall_1min  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'J')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 100 + char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
																								
				if (DATA_check(0, temp_data, 400))
				{
					Weather_data->rainfall_1hour  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'K')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 100 + char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
																											
				if (DATA_check(0, temp_data, 500))
				{
					Weather_data->rainfall_24hours = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'L')
			{
				if (char_to_Number(aprs_data[i + 1]) == '-')
				{
					temp_data = char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
					if (DATA_check(0, temp_data, 100))
					{
						Weather_data->temperature = -temp_data;
					}
					else
					{
						aprs_state = APRS_State_Error;
						break;
					}
				}
				else
				{
					temp_data = char_to_Number(aprs_data[i + 2]) * 10 + char_to_Number(aprs_data[i + 3]) + char_to_Number(aprs_data[i + 4]) * 0.1;
					if (DATA_check(0, temp_data, 100))
					{
						Weather_data->temperature = temp_data;
					}
					else
					{
						aprs_state = APRS_State_Error;
						break;
					}
				}
																														
				
			}
			else if (temp == 'M')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 10 + char_to_Number(aprs_data[i + 2]) + char_to_Number(aprs_data[i + 3]) * 0.1;
																																	
				if (DATA_check(0, temp_data, 100))
				{
					Weather_data->humidity  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 'N')
			{
				temp_data = char_to_Number(aprs_data[i + 1]) * 1000 + char_to_Number(aprs_data[i + 2]) * 100 + char_to_Number(aprs_data[i + 3]) * 10 + char_to_Number(aprs_data[i + 4]) + char_to_Number(aprs_data[i + 5]) * 0.1;
																																				
				if (DATA_check(0, temp_data, 10000))
				{
					Weather_data->AtmosphericPressure  = temp_data;
				}
				else
				{
					aprs_state = APRS_State_Error;
					break;
				}
			}
			else if (temp == 0x0D)
			{
				break;
			}
			else
				continue;
		}
		
		if (aprs_state == APRS_State_Error)
		{
			return;
		}
		aprs_state = APRS_State_Finish;
	}
	else
		return;
	return;
}