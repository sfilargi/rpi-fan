
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bcm2835.h"

#define PIN RPI_GPIO_P1_12
#define PWM_CHANNEL 0
#define RANGE 48

int gettemp (void)
{
  int f;
  char buf[128];
  int n;
  int temp;
  
  f = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
  if (f == -1) {
    printf("Failed opening temp file\n");
    return 100;
  }

  n = read(f, buf, sizeof(buf) - 1);
  if (n < 0) {
    printf("Failed reading temp from file\n");
    close(f);
    return 100;
  }
  buf[n] = '\0';

  temp = atoi(buf);
  temp = temp / 1000;
  if (temp < 0 || temp > 100) {
    close(f);
    return 100;
  }

  close(f);

  return temp;
}

int
getfanpercentage (int temp)
{
  if (temp < 20)
    return 0;

  if (temp > 70)
    return 100;

  temp -= 20;
  
  return (temp * 100) / 50;
}

int
main (int argc, char **argv)
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

    while (1) {
      int temp = gettemp();
      int percentage;
      int value;
      percentage = getfanpercentage(temp);
      value = (percentage * RANGE) / 100;
      printf("Temp: %d, Fan%%: %d, Value: %d/%d\n",
	     temp, percentage, value, RANGE);
      bcm2835_pwm_set_data(PWM_CHANNEL, value);
      usleep(1 * 1000 * 1000);
    }

    bcm2835_close();
    return 0;
}
