/*
 * DIOConfiguration.c
 *
 *  Created on: Oct 31, 2016
 *      Author: is3849
 */

#define PORT_LENGTH 1
#define DATA_ADDRESS 0x288
//#define STATUS_ADDRESS 0x379
//#define CTRL_ADDRESS 0x37a
/* bit 2 = printer initialization (high to initialize) */
/* bit 4 = hardware IRQ (high to enable) */
#define INIT_BIT 0x04
//#define INTR_BIT 0x10
#define PORTA_CTRL (DATA_ADDRESS+11)
//#define PARALLEL_IRQ 0x07 /* parallel port’s interrupt vector */
 uintptr_t data_handle;
 uintptr_t status_handle;
 uintptr_t ctrl_handle;

void config_io()
{
	/* Give this thread root permissions to access */
	/* the hardware */
	privity_err = ThreadCtl( _NTO_TCTL_IO, NULL );
	if ( privity_err == -1)
	{
		printf( "Can't get root permissions\n");
		return -1;
	}
	/* Get a handle to the parallel port's control register */
	ctrl_handle = mmap_device_io( PORT_LENGTH, PORTA_CTRL );
	/* Initialize the parallel port */
	out8( ctrl_handle,  INIT_BIT);

	/* Get a handle to the parallel port's Status Register */
	// don't know what this is yet...
	// status_handle = mmap_device_io( PORT_LENGTH, STATUS_ADDRESS );
	/* Get a handle to the parallel port's Data Register */
	data_handle = mmap_device_io( PORT_LENGTH, DATA_ADDRESS );

	/* eventually you’ll need to: */

	/* Enable interrupts on the parallel port */
	//out8( ctrl_handle, INTR_BIT );
}
