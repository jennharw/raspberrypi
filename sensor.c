#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXTIMINGS 85
#define DHT 7

int dht11[5] = {0,0,0,0,0};
void read_dht11(){
	uint8_t State = HIGH;
	uint8_t Counter = 0;

	uint8_t j =0, i;
        float	Fahrn;
	dht11[0] = dht11[1]= dht11[2] = dht11[3] = dht11[4] = 0;

	pinMode(DHT, OUTPUT);
	digitalWrite(DHT, LOW);
	delay(20);
	digitalWrite(DHT, HIGH);
	delayMicroseconds(40);
	pinMode(DHT, INPUT);

	for (i = 0; i <MAXTIMINGS;i++){
		Counter = 0;
		while (digitalRead(DHT) == State){
			Counter++;
			delayMicroseconds(i);
			if (Counter == 255){
				break;
			}
		}
		State = digitalRead(DHT);
		if (Counter == 255){
			break;
		}
		if ((i >= 4) && (i % 2 == 0)){
				dht11[j/8] << 1;
				if(Counter > 16){
				dht11[j/8] |= 1;
				}
			j++;
			}

	}
	if ((j >= 40) && (dht11[4] == ((dht11[0] + dht11[1] + dht11[2] + dht11[3]) & 0xFF))){
	Fahrn = dht11[2] * 9. / 5. + 32;
	fprintf(stderr, "Himidity = %d.%d   %% \n Temperature = %d.%d c (%.1f F)\n\n", dht11[0], dht11[1], dht11[2], dht11[3], Fahrn);
	}else {
	fprintf(stderr, "Data Error occurred\n");
	}

}

int main(void){
	fprintf(stderr, "Raspberry Pi wiring Pi DHT11 temp & Humidity Measuring \n");
	if (wiringPiSetup() == -1){
		fprintf(stderr, "RPi GPIO setup error...\n");
		exit(1);
	}
	while (1){
		read_dht11();
		delay(1000);
	}

}
