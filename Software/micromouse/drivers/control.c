/*
 * control.c
 *
 *  Created on: Mar 21, 2014
 *      Author: Anthony
 */


#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>

#include "control.h"
#include "system.h"
#include "drivers/motor.h"
#include "drivers/ir_sensor.h"
#include "services/pid.h"
#include "services/time_keeper.h"



Semaphore_Handle drive_straight_sem_handle;
Semaphore_Params drive_straight_sem_params;

pid_controller_t side_pid;

void drive_straight(){

	update_motor(RIGHT_MOTOR, CCW, SPEED);
	update_motor(LEFT_MOTOR, CW, SPEED);

	side_ir_data_t side_data;
	uint32_t left_avg;
	uint32_t right_avg;
	int32_t side_diff;

	float motor_diff;

	int16_t right_motor_out;
	int16_t left_motor_out;


	while(1){

		Semaphore_pend(drive_straight_sem_handle, BIOS_WAIT_FOREVER);

		side_poll(&side_data);

		left_avg = (side_data.left_back + side_data.left_front)/2;
		right_avg = (side_data.right_back + side_data.right_front)/2;

		side_diff = left_avg - right_avg;

		motor_diff = pid_step(&side_pid, SETPOINT, (float)side_diff, (float)get_curr_time_us())/100;

		right_motor_out = SPEED + motor_diff/2;
		left_motor_out = SPEED - motor_diff/2;

		if(right_motor_out < 0){
			update_motor(RIGHT_MOTOR, CW, -1*right_motor_out);
		}
		else{
			update_motor(RIGHT_MOTOR, CCW, right_motor_out);
		}

		if(left_motor_out < 0){
			update_motor(LEFT_MOTOR, CCW, -1*left_motor_out);
		}
		else{
			update_motor(LEFT_MOTOR, CW, left_motor_out);
		}
	}

}

void control_init(){

	time_keeper_init();
	pid_init(&side_pid, KP, KI, KD, (float)get_curr_time_us());

	Semaphore_Params_init(&drive_straight_sem_params);
	drive_straight_sem_params.mode = Semaphore_Mode_BINARY;
	drive_straight_sem_handle = Semaphore_create(0, &drive_straight_sem_params, NULL);
}



void drive_straight_resume(void){
	// Resume the task
	Semaphore_post(drive_straight_sem_handle);
}