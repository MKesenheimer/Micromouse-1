#ifndef PWM_MOTOR_H
#define PWM_MOTOR_H

#include <system_include.h>

// CPU/MOTOR Configurations
// CPU_FREQ/64
#define PWM_TIMER_FREQ 1250000//Hz

#define NUM_MOTORS 2
#define MOTOR_PERIOD 50000

// Holds PWM information for each motor
typedef struct pwm_motor_config_t {
	uint32_t pwm_base_module;
	uint32_t pwm_generator;
	uint32_t pwm_pin;
} pwm_motor_config_t;

// Define config information for the motors
static pwm_motor_config_t pwm_motors[NUM_MOTORS] = {
	{// Right Motor (PF2)
		PWM1_BASE,
		PWM_GEN_3,
		PWM_OUT_6
	},
	{// Left Motor (PF3)
		PWM1_BASE,
		PWM_GEN_3,
		PWM_OUT_7
	}
};

/*
 * Initialize PWM pins and register with system using GPIO libraries.
 * NOTE: If a new motor is added to the pwm_motors list, please make
 * 		 it is initialized in this function.
 */
void pwm_gpio_init(void);

/*
 * Sets the duty cycle to the closest approximation of uSec that can be set.
 * After the function returns, the uSec variable will be changed to the approximation
 * that the pulse width was set to.
 */
void set_pulse_width(uint8_t motorIndex, uint16_t *uSec);

void set_pwm_state(bool isOn);
#endif