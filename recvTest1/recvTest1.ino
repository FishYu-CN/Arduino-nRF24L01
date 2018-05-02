/**
* An Mirf example which copies back the data it recives.
*
* Pins:
* Hardware SPI:
* MISO -> 12
* MOSI -> 11
* SCK -> 13
*
* Configurable:
* CE -> 8
* CSN -> 7
*
*/

#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"

unsigned char RADDR[5] = { 0x34,0x43,0x10,0x10,0x01 };

void setup() {
	Serial.begin(9600);

	/*
	* Set the SPI Driver.
	*/

	Mirf.spi = &MirfHardwareSpi;

	/*
	* Setup pins / SPI.
	*/

	Mirf.init();

	/*
	* Configure reciving address.
	*/

	Mirf.setRADDR(RADDR);

	/*
	* Set the payload length to sizeof(unsigned long) the
	* return type of millis().
	*
	* NB: payload on client and server must be the same.
	*/


	/*
	* Write channel and payload config then power up reciver.
	*/
	Mirf.cePin = 8;
	Mirf.csnPin = 7;
	Mirf.payload = 32;
	Mirf.channel = 0;

	Mirf.config();

	Serial.println("Listening...");
}

void loop() {
	/*
	* A buffer to store the data.
	*/

	byte data[Mirf.payload];

	/*
	* If a packet has been recived.
	*
	* isSending also restores listening mode when it
	* transitions from true to false.
	*/

	if (!Mirf.isSending() && Mirf.dataReady()) {
		Serial.println("Got packet");

		/*
		* Get load the packet into the buffer.
		*/

		Mirf.getData(data);
		for(int i=0;i<data[0];i++)
		Serial.print((char)data[i+1]);
		/*
		* Set the send address.
		*/


		//Mirf.setTADDR(RADDR);

		/*
		* Send the data back to the client.
		*/

		//Mirf.send(data);

		/*
		* Wait untill sending has finished
		*
		* NB: isSending returns the chip to receving after returning true.
		*/

		//Serial.println("Reply sent.");
	}
}