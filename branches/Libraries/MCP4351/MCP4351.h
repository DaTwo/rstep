/*Library for MCP4351 and CNC project rStep by Reza Naima.  */
/*												*/
/*Library written by Michael Nash and based heavily on		*/
/*original code in rStep Firmware written by Reza.		*/
/*												*/


#ifndef MCP4351_H
#define MCP4351_H

#include "WProgram.h"

/*Pin assignments*/
#ifdef ATMEGA644 || ATMEGA644P
#define _MCP_MISO	6	//PIN B6
#define _MCP_MOSI	5	//PIN B5
#define _MCP_SS	4	//PIN B4
#define _MCP_SCK	7	//PIN B7
#endif

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
#ifndef MAX_CURRENT
#define MAX_CURRENT 8.0
#endif
//#define MAX_CURRENT 2.0

class MCPClass{
public:
	void init();
	uint8_t setWiper(uint8_t wiper, uint16_t value);
	uint8_t setCurrent(uint8_t channel, float current);
	uint8_t write(uint8_t address);
	uint8_t read(uint8_t address);
	
private:
	uint8_t _SPI_TX(uint8_t x);
	
};

extern MCPClass MCP;

#endif

