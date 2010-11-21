/*Library for MCP4351 and CNC project rStep by Reza Naima.  */
/*												*/
/*Library written by Michael Nash and based heavily on		*/
/*original code in rStep Firmware written by Reza.		*/
/*												*/

#include "MCP4351.h"
#include "WProgram.h"
#include "avr/io.h"

/*Pin assignments*/
//#ifdef ATMEGA644 || ATMEGA644P
#define _MCP_MISO	6	//PIN B6
#define _MCP_MOSI	5	//PIN B5
#define _MCP_SS	4	//PIN B4
#define _MCP_SCK	7	//PIN B7
					//#endif

/*General Utilies*/
#define MISO_STATE bit_is_set(PINB,_MCP_MISO)
#define MOSI_STATE bit_is_set(PINB,_MCP_MOSI)
#define SCK_STATE bit_is_set(PINB,_MCP_SCK)
#define SS_STATE bit_is_set(PINB,_MCP_SS)
#define _MISO(x) (x<<_MCP_MISO)
#define _MOSI(x) (x<<_MCP_MOSI)
#define _SCK(x) (x<<_MCP_SCK)
#define _SS(x) (x<<_MCP_SS)
#define SPI_ON() SPCR = _BV(MSTR) | _BV(SPE)
#define SPI_OFF() SPCR &= ~_BV(SPE);

/*MCP4351 commands*/
#define SPI_CLOCK_DIV2 0x04
#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C
#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPS

/*Specific maximum set for rStep*/
#define MAX_CURRENT 8.0
//#define MAX_CURRENT 2.0

/*
 
 8-bit command byte:
 
 [AD3|AD2|AD1|AD0|C1|C0|D9|D8]
 [MEMORY-ADDRESS |COMND| DATA]
 
 
 
 16-bit command

[AD3|AD2|AD1|AD0|C1|C0|D9|D8}-{D7|D6|D5|D4|D3|D2|D1|D0]
[MEMORY-ADDRESS |COMND|         DATA BITS		    ]
 
 */
/***************************************************/
/***************************************************/

MCPClass MCP;

void MCPClass::init() {
	//configure pins
	pinMode(_MCP_SS, OUTPUT);
	pinMode(_MCP_MISO, INPUT);

	digitalWrite(_MCP_SCK, HIGH);
	digitalWrite(_MCP_MOSI, HIGH);
	digitalWrite(_MCP_MISO, HIGH);
		
	DDRB |= (1<<_MCP_MOSI)|(1<<_MCP_SCK);	//MOSI and SCK pins set to output
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);	//SPI Enabled as Master, Clock rate fck/16
	PORTB |= (1<<_MCP_SS);
	SPDR = 0x01;						//wake up the MCP4351 with a byte
	PORTB |= (0<<_MCP_SS);
}

uint8_t MCPClass::setWiper(uint8_t wiper, uint16_t value) {
	uint8_t a,b;
	uint8_t addrs[4] = {(0<<4),(1<<4),(6<<4),(7<<4)};
	value &= 0x1FF;
	SPI_ON();
	digitalWrite(_MCP_SS, LOW);
	a=_SPI_TX(addrs[wiper] | (value>>8));
	b=_SPI_TX(value & 0xFF);
	digitalWrite(_MCP_SS, HIGH);
	SPI_OFF();
	if (a!=0xFF || b!=0xFF) {
		Serial.println("ERR CUR");
		return false;
	}
	return true;
}

uint8_t MCPClass::setCurrent(uint8_t channel, float current) {
	if (current > MAX_CURRENT){
//		Serial.println("Warning:  Current setting higher than current limit.");
//		Serial.println("Defaulting to 2.0A.");
//		Serial.print("Current limit ");
//		Serial.print(MAX_CURRENT);
//		Serial.println("A");
		uint16_t val = (0x100*(2.0/MAX_CURRENT));
		uint8_t cc = (setWiper(channel, val)) ?  (val>>1):0;		
//		return cc;
		return false;
	}
	digitalWrite(_MCP_SS,LOW);		
	uint16_t val = (0x100*(current/MAX_CURRENT));
	uint8_t cc = (setWiper(channel, val)) ?  (val>>1):0; 
	digitalWrite(_MCP_SS,HIGH);	
	return cc;
}

uint8_t MCPClass::read(uint8_t address){
	/*to do maybe*/
	return 0;
}

uint8_t MCPClass::write(uint8_t address){
	/*to do maybe*/
	return 0;
}


/*PRIVATE*/
uint8_t MCPClass::_SPI_TX(uint8_t x) {
	SPDR = x;
	while (!(SPSR & _BV(SPIF)));
		return SPDR;
}

