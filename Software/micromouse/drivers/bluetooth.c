/*
 * bluetooth.c
 *
 *  Created on: Mar 18, 2014
 *      Author: Anthony
 */


#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_ints.h>
#include <inc/hw_gpio.h>

#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>

#include <xdc/runtime/System.h>

#include "system.h"
#include "bluetooth.h"

#include <ti/drivers/uart/UARTTiva.h>
#include <ti/drivers/UART.h>


typedef struct {
	UART_Handle handle;
	UART_Params params;
} bluetooth_t;


bluetooth_t bluetooth;

void bluetooth_init() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
	GPIOPinConfigure(GPIO_PC4_U4RX);
	GPIOPinConfigure(GPIO_PC5_U4TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
}

void bluetooth_open(){
	UART_Params_init(&(bluetooth.params));
	bluetooth.params.baudRate  = 115200;
	bluetooth.params.writeDataMode = UART_DATA_BINARY;
	bluetooth.params.readDataMode = UART_DATA_BINARY;
	bluetooth.params.readReturnMode = UART_RETURN_FULL;
	bluetooth.params.readEcho = UART_ECHO_OFF;
	bluetooth.handle = UART_open(BLUETOOTH, &(bluetooth.params));
	if (!bluetooth.handle) {
		System_printf("UART did not open");
	}
}

void bluetooth_transmit(char *data, int len) {
	int ret = UART_write(bluetooth.handle, data, len);
	System_printf("The UART wrote %d bytes\n", ret);
}

void bluetooth_receive(char *data, int len) {
	int ret = UART_read(bluetooth.handle, data, len);
	//System_printf("The UART read %d bytes\n", ret);
}
