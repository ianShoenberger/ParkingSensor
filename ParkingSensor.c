#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <hw/inout.h>
#include "DIOConfiguration.h"

#define PING_DUTY_NANOSECONDS (10100)
#define SENSOR_READING_UPPER_LIMIT (18000000)

int clock_speed;
int ping_duty_cycle;

void ping(void);
int read_distance(void);
int convert_sensor_reading_to_inches(long sensor_reading);

int main(int argc, char *argv[]) {
	int config_status = 0;
	int distance_reading;
	int max_reading = 0;
	int min_reading = 0;
	struct timespec wait_time;
	wait_time.tv_sec = 0;
	wait_time.tv_nsec = 82000000;

	printf("Running the Parking Sensor Application\n");

	config_status = config_io();
	if ( config_status != -1 )
	{
		printf("Successfully configured digital IO\n");
		while(1)
		{
			ping();
			distance_reading = read_distance();
			if(distance_reading == -1) {
				printf("*\n");
			} else {
				printf("%d\n", distance_reading);

				if(distance_reading > max_reading) {
					max_reading = distance_reading;
				} else if(distance_reading <= min_reading) {
					min_reading = distance_reading;
				}
			}

			nanosleep(&wait_time, NULL);
			//sleep(1);
		}
	}
	else
	{
		printf("Configuration of digital IO Unsuccessful\n");
	}

	return EXIT_SUCCESS;
}

void ping(void)
{
	struct timespec ping_duty_cycle;
	ping_duty_cycle.tv_sec = 0;
	ping_duty_cycle.tv_nsec = 0;
	// we set the output to be 1 and then wait..
	out8( pa_data_handle, HIGH );
	ping_duty_cycle.tv_nsec = PING_DUTY_NANOSECONDS;

	nanosleep(&ping_duty_cycle, NULL);
	out8( pa_data_handle, LOW );
}

int read_distance(void)
{
	struct timespec start_time;
	struct timespec end_time;
	struct timespec curr_time;
	long sensor_read_time = 0;
	int sensor_read_inches = 0;

	start_time.tv_sec = 0;
	start_time.tv_nsec = 0;
	end_time.tv_sec = 0;
	end_time.tv_nsec = 0;
	curr_time.tv_sec = 0;
	curr_time.tv_nsec = 0;

	// wait for the input to go up to 1
	while((in8(pb_data_handle) & PB0_BIT) != PB0_BIT){}
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	clock_gettime(CLOCK_MONOTONIC, &curr_time);

	while((in8(pb_data_handle) & PB0_BIT) != 0){
		// check to see if we have waited too long
		/*if((curr_time.tv_nsec - start_time.tv_nsec) > SENSOR_READING_UPPER_LIMIT) {
			return -1;
		}
		clock_gettime(CLOCK_MONOTONIC, &curr_time);*/
	}
	clock_gettime(CLOCK_MONOTONIC, &end_time);

	sensor_read_time = end_time.tv_nsec - start_time.tv_nsec;
	//printf("%lu - time interval\n", sensor_read_time);
	sensor_read_inches = convert_sensor_reading_to_inches(sensor_read_time);

	return sensor_read_inches;
}

int convert_sensor_reading_to_inches(long sensor_reading) {
	// convert to ms from ns
	float ms_sensor_reading = (float)sensor_reading / 1000000.00;
	float result = (10.80*ms_sensor_reading) / 2.00;
	return ceil(result);
}
