test:	blutooth.o main.o speaker.o switch.o display.o lightSensor.o checkAlarm.o
	gcc -o test blutooth.o main.o speaker.o switch.o display.o lightSensor.o checkAlarm.o -lwiringPi -lpthread

blutooth.o: blutooth.c common.h
	gcc -c blutooth.c -lwiringPi -lpthread

main.o: main.c common.h
	gcc -c main.c -lwiringPi -lpthread

speaker.o: speaker.c common.h
	gcc -c speaker.c -lwiringPi -lpthread

switch.o: switch.c common.h
	gcc -c switch.c -lwiringPi -lpthread

display.o: display.c common.h
	gcc -c display.c -lwiringPi -lpthread

lightSensor.o: lightSensor.c common.h
	gcc -c lightSensor.c -lwiringPi -lpthread

checkAlarm.o: checkAlarm.c common.h
	gcc -c checkAlarm.c -lwiringPi -lpthread

