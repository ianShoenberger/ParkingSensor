#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "DIOConfiguration.h"

int clock_speed;
int ping_duty_cycle;

void ping(void);
int read(void);

int main(int argc, char *argv[]) {
	int config_status = 0;
	int distance_reading = 0;

	// we want to get the num clock cycles and then divide this to know how long to leave ping duty cycle
	// 'up for' since it needs to at least be 10us
	clock_speed = ClockPeriod(CLOCK_REALTIME);
	ping_duty_cycle = clock_speed/100000;
	printf("Running the Parking Sensor Application\n");

	config_status = config_io();
	if ( config_status != -1 )
	{
		printf("Successfully configured digital IO\n");
		while(1)
		{
			ping();
			distance_reading = read();
			printf("The num seconds %f.2d", distance_reading);
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
	int start_time = 0;
	// we set the output to be 1 and then wait..
	out8( pa_data_handle, HIGH );
	start_time = ClockCycles();
	while( (ClockCycles() - start_time)  < ping_duty_cycle ){}
	out8( pa_data_handle, LOW );
}

int read(void)
{
	int start_time = 0;
	int distance_period = 0;
	int sensor_read_seconds = 0;

	while(pb_data_handle & PB_BIT != PB_BIT){}

	start_time = ClockCycles();
	// eventually need to put a limit to how long we wait here
	while(pb_data_handle & PB_BIT != 0){}
	distance_period = ClockCycles() - start_time;
	sensor_read_seconds = distance_period / clock_speed;
	return sensor_read_seconds;
}
