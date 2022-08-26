.DEFAULT_GOAL=rpi-fan


pwm.o: pwm.c bcm2835.h
	gcc -c -Wall -Werror -o pwm.o pwm.c

bcm2835.o: bcm2835.c bcm2835.h
	gcc -c -Wall -Werror -o bcm2835.o bcm2835.c

rpi-fan: pwm.o bcm2835.o
	gcc -Wall -Werror -o rpi-fan bcm2835.o pwm.o
