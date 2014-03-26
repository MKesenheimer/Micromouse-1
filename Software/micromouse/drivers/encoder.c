/*
 * encoder.c
 *
 *  Created on: Mar 22, 2014
 *      Author: Anthony
 */

#include "stdbool.h"
#include "stdint.h"
#include <stdlib.h>
#include <stdio.h>

#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include <driverlib/gpio.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include "encoder.h"
#include "drivers/bluetooth.h"

ti_sysbios_family_arm_m3_Hwi_Struct left_encoder_hwi;
Hwi_Params left_encoder_hwi_params;

ti_sysbios_family_arm_m3_Hwi_Struct right_encoder_hwi;
Hwi_Params right_encoder_hwi_params;

uint32_t left_motor_ticks = 0;
uint32_t right_motor_ticks = 0;

bool stream_buf_encoder = false;
char spf_buf_encoder[80];

uint8_t check_walls = 0;

void encoder_init(){

	SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0 );
	SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC1 );

	// Select the analog ADC function for these pins.
	GPIOPinTypeADC( GPIO_PORTE_BASE, GPIO_PIN_0);	//CH3 - Right Encoder B
	GPIOPinTypeADC( GPIO_PORTE_BASE, GPIO_PIN_1);	//CH2 - Right Encoder A
	GPIOPinTypeADC( GPIO_PORTE_BASE, GPIO_PIN_2);	//CH1 - Left Encoder B
	GPIOPinTypeADC( GPIO_PORTE_BASE, GPIO_PIN_3);	//CH0 - Left Encoder A

	/**
	 * Set up ADC1 sequence 1 for operation for the encoders
	 *
	 * Uses Comparator 0 and 1 to check for rising edge
	 *
	 * CH0 - PE3 - Left Encoder A
	 *
	 */
	ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_ALWAYS, 3);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH0|ADC_CTL_CMP0);
	ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH0|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 1);

	//Set up the comparator for Left Encoder A
	ADCComparatorConfigure(ADC1_BASE, 0, ADC_COMP_TRIG_NONE|ADC_COMP_INT_HIGH_HONCE );
	ADCComparatorRegionSet(ADC1_BASE, 0, ENCODER_MAX_LOW, ENCODER_MAX_HIGH);
	ADCComparatorReset(ADC1_BASE, 0, true, true);
	ADCComparatorIntEnable(ADC1_BASE, 1);

	// Set up Hardware Interrupt for ADC
	Hwi_Params_init(&left_encoder_hwi_params);
	//encoder_hwi_params.arg = (UArg)adc_hwi;
	Hwi_construct(&left_encoder_hwi, INT_ADC1SS1, left_encoder_count, &left_encoder_hwi_params, NULL);

	/**
	 * Set up ADC1 sequence 1 for operation for the encoders
	 *
	 * Uses Comparator 0 and 1 to check for rising edge
	 *
	 * CH2 - PE1 - Right Encoder A
	 *
	 */
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_ALWAYS, 3);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH2|ADC_CTL_CMP0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 1, ADC_CTL_CH2|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);

	//Set up the comparator for Right Encoder A
	ADCComparatorConfigure(ADC0_BASE, 0, ADC_COMP_TRIG_NONE|ADC_COMP_INT_HIGH_HONCE );
	ADCComparatorRegionSet(ADC0_BASE, 0, ENCODER_MAX_LOW, ENCODER_MAX_HIGH);
	ADCComparatorReset(ADC0_BASE, 0, true, true);
	ADCComparatorIntEnable(ADC0_BASE, 3);

	// Set up Hardware Interrupt for ADC
	Hwi_Params_init(&right_encoder_hwi_params);
	//encoder_hwi_params.arg = (UArg)adc_hwi;
	Hwi_construct(&right_encoder_hwi, INT_ADC0SS3, right_encoder_count, &right_encoder_hwi_params, NULL);

}

void encoder_open() {
	// Nothing here
}


void left_encoder_count(){

	ADCComparatorIntClear(ADC1_BASE, 0x0F);

	left_motor_ticks++;

	check_distance();

}


void right_encoder_count(){

	ADCComparatorIntClear(ADC0_BASE, 0x0F);

	right_motor_ticks++;

	check_distance();

}

void check_distance(){

	uint32_t avg_ticks;
	avg_ticks = (right_motor_ticks + left_motor_ticks)/2;

	if( (avg_ticks >= NUMTICKS_PER_BLOCK/2) & (avg_ticks <= NUMTICKS_PER_BLOCK + WALL_CHECK_ZONE)){
		check_walls = 1;
	}
	else{
		check_walls = 0;
	}

	if((avg_ticks)/2 >= NUMTICKS_PER_BLOCK){
		//DO MOVE
		left_motor_ticks = 0;
		right_motor_ticks = 0;
	}

	if(stream_buf_encoder){
		int len = sprintf(spf_buf_encoder, "R: %i, L: %i\r\n", right_motor_ticks, left_motor_ticks);
		bluetooth_transmit(spf_buf_encoder, len);
	}
}

void stream_encoder(char* val) {
	if(strcmp(val, "on") == 0) {
		stream_buf_encoder = true;
	} else {
		if(strcmp(val, "off") == 0) {
			stream_buf_encoder = false;
		} else {
			bluetooth_transmit("Invalid Encoder Stream Value! 'on' or 'off'\r\n", 36);
		}
	}
}
