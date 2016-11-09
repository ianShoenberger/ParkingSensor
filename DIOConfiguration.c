/*
 * DIOConfiguration.c
 *
 *  Created on: Oct 31, 2016
 *      Author: is3849
 */

#include "DIOConfiguration.h"

int config_io(void)
{
	int privity_err;
	int dio_direction;

	/* Give this thread root permissions to access */
	/* the hardware */
	privity_err = ThreadCtl( _NTO_TCTL_IO, NULL );
	if ( privity_err == -1)
	{
		printf( "Can't get root permissions\n");
		return -1;
	}
	/* Get a handle to the parallel port's control register */
	ctrl_handle = mmap_device_io( PORT_LENGTH, CTRL_ADDRESS );
	/* Initialize port A to be output */
	dio_direction = in8( ctrl_handle );
	out8( ctrl_handle,  dio_direction & ~PA_DIR_BIT);
	/* Initialize port B to be an input */
	dio_direction = in8( ctrl_handle );
	out8( ctrl_handle, dio_direction | PB_DIR_BIT);


	/* Get a handle to the parallel port's Data Register */
	pa_data_handle = mmap_device_io( PORT_LENGTH, PA_DATA_ADDRESS );
	pb_data_handle = mmap_device_io( PORT_LENGTH, PB_DATA_ADDRESS );

	return 1;

}

void output_pwm(void)
{
	int count;

	for ( count = 0; count < MAX_COUNT; count++ )
	{
		/* Output a byte of lows to the data lines */
		out8( pb_data_handle, LOW );
		printf("low\n");
		sleep( 1 );

		/* Output a byte of highs to the data lines */
		out8( pb_data_handle, HIGH );
		printf("high\n");
		sleep( 1 );
	}
}

void input_pwm(void)
{
	while(1)
	{
		while((in8(pb_data_handle) & PB0_BIT) != PB0_BIT){}
		printf("input has gone high\n");

		while((in8(pb_data_handle) & PB0_BIT) != 0){}
		printf("input has gone low\n");
	}

}
