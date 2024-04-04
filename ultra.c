#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define BUZZ 1
#define TRIG 4
#define ECHO 5

void setup(){
	if (wiringPiSetup() == -1){
	fprintf(stderr, "wiring pi. rror,\n");
	exit(0);
	}
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);

	digitalWrite(TRIG, LOW);
	delay(30);
}

int getCM(){
	//send trigger pulse
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIG, LOW);

	//wait for echo start
	long startTime = micros();
	while(digitalRead(ECHO) == HIGH){
		continue;
	}
	long travelTime = micros() - startTime;
	int distance = travelTime / 58;
	return distance;

}
int main(){
	int prev = 0;
	setup();
	while(1){
		int dist = getCM();
		//if (abs(dist-prev) >=2) {
			fprintf(stderr, "Distance %d cm\n", dist);
		//
		//}
		fprintf(stderr,"%d", digitalRead(ECHO));
		delay(1000);

	}
	return 0;
}
