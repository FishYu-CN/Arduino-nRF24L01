/**
* A Mirf example to test the latency between two Ardunio.
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
* Note: To see best case latency comment out all Serial.println
* statements not displaying the result and load
* 'ping_server_interupt' on the server.
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
#define ONE_WIRE_BUS A0
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
unsigned char TADDR[5] = { 0x34,0x43,0x10,0x10,0x01 };
unsigned long time;
uint8_t sendbuf[32], recvbuf[32];
void setup() {
	Serial.begin(9600);
	/*
	* Setup pins / SPI.
	*/

	/* To change CE / CSN Pins:
	*
	* Mirf.csnPin = 9;
	* Mirf.cePin = 7;
	*/
	pinMode(2, OUTPUT);
	digitalWrite(2, HIGH);
	sensors.begin();
	Mirf.cePin = 8;
	Mirf.csnPin = 7;
	Mirf.payload = 32;
	Mirf.channel = 0;
	Mirf.spi = &MirfHardwareSpi;
	Mirf.init();

	Serial.println("Beginning ... ");

	Mirf.config();

	Mirf.ceLow();
	//Mirf.writeRegister(TX_ADDR, TADDR, 5);
	//Mirf.readRegister(TX_ADDR, buf, 5);
	//Serial.print(buf[0], HEX);
	//Serial.print(buf[1], HEX);
	//Serial.print(buf[2], HEX);
	//Serial.print(buf[3], HEX);
	//Serial.println(buf[4], HEX);

	//Mirf.writeRegister(RX_ADDR_P0, TADDR, 5);
	//Mirf.readRegister(RX_ADDR_P0, buf, 5);
	//Serial.print(buf[0], HEX);
	//Serial.print(buf[1], HEX);
	//Serial.print(buf[2], HEX);
	//Serial.print(buf[3], HEX);
	//Serial.println(buf[4], HEX);

	//Mirf.configRegister(EN_AA, 0x01);
	//Mirf.readRegister(EN_AA, buf, 1);
	//Serial.println(buf[0], HEX);

	//Mirf.configRegister(EN_RXADDR, 0x01);
	//Mirf.readRegister(EN_RXADDR, buf, 1);
	//Serial.println(buf[0], HEX);

	Mirf.configRegister(SETUP_RETR, 0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
										  //Mirf.readRegister(SETUP_RETR, buf, 1);
										  //Serial.println(buf[0], HEX);

										  //Mirf.configRegister(RF_CH, 0);
										  //Mirf.readRegister(RF_CH, buf, 1);
										  //Serial.println(buf[0], HEX);

										  //Mirf.configRegister(RF_SETUP, 0x0f);
										  //Mirf.readRegister(RF_SETUP, buf, 1);
										  //Serial.println(buf[0], HEX);

										  //Mirf.configRegister(CONFIG, 0x0e);
										  //Mirf.readRegister(CONFIG, buf, 1);
										  //Serial.println(buf[0], HEX);

	Mirf.ceHi();

	//sendbuf[32] = 0;
	//Mirf.ceLow();
	//Mirf.csnLow();
	//Mirf.spi->transfer(W_TX_PAYLOAD);
	//Mirf.transmitSync(sendbuf,32);
	//Mirf.csnHi();
	////Mirf.readRegister(W_TX_PAYLOAD, buf, 32);
	////Serial.print(buf[0], HEX);
	////Serial.print(buf[1], HEX);
	////Serial.print(buf[2], HEX);
	////Serial.print(buf[3], HEX);
	////Serial.println(buf[4], HEX);

	//Mirf.ceHi();
	//delay(100);
	//Mirf.ceLow();
	//Mirf.readRegister(STATUS, buf, 1);  //读取状态寄存器的值	   
	//Serial.println(buf[0], HEX);
	//Mirf.readRegister(FIFO_STATUS, buf, 1);  //读取状态寄存器的值	   
	//Serial.println(buf[0], HEX);
	//Mirf.ceLow();

	sendbuf[0] = 11;
	sendbuf[1] = 'C';
	sendbuf[2] = '2';
	sendbuf[3] = ':';
	sendbuf[4] = 'H';
	sendbuf[5] = '0';
	sendbuf[6] = '5';
	sendbuf[7] = '0';
	sendbuf[8] = 'T';
	sendbuf[9] = '0';
	sendbuf[10] = '5';
	sendbuf[11] = '0';

}

void loop() {

	unsigned int raw,rawTemp;
	sensors.requestTemperatures(); // Send the command to get temperatures
	Serial.println(rawTemp=sensors.getTempCByIndex(0));
	Mirf.setTADDR(TADDR);
	raw = analogRead(A1);

	raw = map(raw, 0, 1023, 100, 0);
	Serial.println(raw);
	sendbuf[5] = (raw / 100) + '0';
	sendbuf[6] = ((raw / 10)%10) + '0';
	sendbuf[7] = (raw % 10) + '0';
	sendbuf[9] = (rawTemp / 100) + '0';
	sendbuf[10] = ((rawTemp / 10)%10) + '0';
	sendbuf[11] = (rawTemp % 10) + '0';
	Mirf.send(sendbuf);
	while (Mirf.isSending()) {
	}

	Serial.println("Finished sending");
	//sendbuf[1]=='z'?sendbuf[1] = 'a': sendbuf[1]++;
	time = millis();
	while (!Mirf.dataReady())
	{
		if ((millis() - time) > 1000)
			break;
	}
	if (!Mirf.isSending() && Mirf.dataReady()) {
		Serial.println("Got packet");

		/*
		* Get load the packet into the buffer.
		*/

		Mirf.getData(recvbuf);

		if (recvbuf[2] == '2')
		{
			if (recvbuf[4] == 'O')
				digitalWrite(2, LOW);
			else
				digitalWrite(2, HIGH);
		}

		for (int i = 0;i<recvbuf[0];i++)
			Serial.print((char)recvbuf[i + 1]);
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
	delay(1000);
}


