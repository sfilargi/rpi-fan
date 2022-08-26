
#include <stdio.h>

#include "bcm2835.h"

#define PIN RPI_GPIO_P1_12
#define PWM_CHANNEL 0
#define RANGE 48

int main(int argc, char **argv)
{
    if (!bcm2835_init())
	return 1;

    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_ALT5);

    // Clock divider is set to 16.
    // With a divider of 16 and a RANGE of 1024, in MARKSPACE mode,
    // the pulse repetition frequency will be
    // 1.2MHz/48 = 25kHz, suitable for PWM Fan
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);

    bcm2835_pwm_set_data(PWM_CHANNEL, 6);

    bcm2835_close();
    return 0;
}
