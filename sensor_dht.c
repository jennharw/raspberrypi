// v2020.01.18

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BIT_LIMIT 40

bool    DATA_ERROR = 0;
uint8_t MAXBITS =  BIT_LIMIT;                   // max. number bits to read (1 bit - 2 changes)
uint8_t raw_data[BIT_LIMIT];                    // 40 byte raw catched data
uint8_t dht11_data[5];
uint8_t DATA_ERROR_RETRY = 5;                   // maximum retry attempt
int     RETRY_WAIT = 2500;                      // wait in msec between retries
uint8_t MCU_START_LOW_WAIT = 18;                // MCU start low level signal length in msec
uint8_t BIT_MARKER = 45;                        // Decision (the bit is 0 if less or 1 if more) level in usec.
uint8_t MAX_USEC_BIT_WAIT = 120;                // MAX wait for reading signal changes in usec
uint8_t INC_STEP = 10;                          // The size off delay increments

int read_dht11_dat(int DHTPIN)
{
        uint8_t counter;
        uint8_t i;
        uint8_t tmp1;
        uint8_t BITS = MAXBITS;

        // MCU start communication cycle
        pinMode( DHTPIN, OUTPUT );                      // set pin mode to write
        digitalWrite( DHTPIN, LOW );                    // set pin LOW
        delay( MCU_START_LOW_WAIT );                    // start signal length
        digitalWrite( DHTPIN, HIGH );                   // set pin HIGH
        pinMode( DHTPIN, INPUT );                       // set pin mode to read

        // wait for DHT start cycle LOW level
        counter=0;
        while ( digitalRead( DHTPIN ) == HIGH )
        {
          counter=counter+INC_STEP;
          delayMicroseconds( INC_STEP );
          if ( counter >= MAX_USEC_BIT_WAIT )
          {
            DATA_ERROR = 1;
            break;
          }
        }

        // wait for DHT start cycle HIGH level
        counter=0;
        while ( digitalRead( DHTPIN ) == LOW )
        {
          counter=counter+INC_STEP;
          delayMicroseconds( INC_STEP );
          if ( counter >= MAX_USEC_BIT_WAIT )
          {
            DATA_ERROR = 1;
            break;
          }
        }

        // wait for DHT data transmission (wait for DHT LOW level)
        counter=0;
        while ( digitalRead( DHTPIN ) == HIGH )
        {
          counter=counter+INC_STEP;
          delayMicroseconds( INC_STEP );
          if ( counter >= MAX_USEC_BIT_WAIT )
          {
            DATA_ERROR = 1;
            break;
          }
        }

        for (i = 0; i < MAXBITS; i++)
        {
          // wait for HIGH level
          counter = 0;
          while ( digitalRead( DHTPIN ) == LOW )
          {
            counter=counter+INC_STEP;
            delayMicroseconds( INC_STEP );
            if ( counter >= MAX_USEC_BIT_WAIT )
            {
              DATA_ERROR = 1;
              break;
            }
          }

          // wait for LOW level
          counter = 0;
          while ( digitalRead( DHTPIN ) == HIGH )
          {
            counter=counter+INC_STEP;
            delayMicroseconds( INC_STEP );
            if ( counter >= MAX_USEC_BIT_WAIT )
            {
              DATA_ERROR = 1;
              break;
            }
          }
          raw_data[i] = counter;
        }

        // analizing raw_data
        for (i = 0; i < MAXBITS; i++)
        {
          tmp1 = i / 8;
          dht11_data[tmp1] <<= 1;
          if ( raw_data[i] > BIT_MARKER ) dht11_data[tmp1] |= 1;
        }

        if ( dht11_data[4] == ( dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) )
        {
          printf( "Humidity : %d.%d %%\n", dht11_data[0], dht11_data[1] );
          printf( "Temperature(F) : %d.%d C (%.1f F)\n", dht11_data[2], dht11_data[3], dht11_data[4] );
          DATA_ERROR_RETRY=0;
          DATA_ERROR = 0;
        } else  { DATA_ERROR = 1; }
}

int main( int argc, char *argv[] )
{
        if ( wiringPiSetup() == -1 ) exit( 1 );         // for initialising wiringPi
        if( argc != 1 ) {
          printf("The GPIO port number is required!\n");
          printf("When use GPIO.0: sudo ./read_DHT11 0.\n");
          printf("Use \"gpio readall\" and check wPi port numbers.\n");
          return 0;
        }
        while(1){
		read_dht11_dat(7);
		delay(1000);
	}
	while ( DATA_ERROR_RETRY > 0 )
        {
          read_dht11_dat(7);
          if ( DATA_ERROR )
          {
            DATA_ERROR_RETRY--;
            delay( RETRY_WAIT );
          } else { return 0; }
        }
        if ( DATA_ERROR ) { return 1; } else { return 0; }
}

