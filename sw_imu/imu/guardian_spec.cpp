#include "guardian.h"

template <>
bool Guardian<CC1101>::init(){
	// Sync word qualifier mode = 30/32 sync word bits detected 
	// CRC autoflush = false 
	// Channel spacing = 49.987793 
	// Data format = Normal mode 
	// Data rate = 499.878 
	// RX filter BW = 541.666667 
	// PA ramping = false 
	// Preamble count = 8 
	// Whitening = false 
	// Address config = No address check 
	// Carrier frequency = 916.500000 
	// Device address = 0 
	// TX power = 0 
	// Manchester enable = false 
	// CRC enable = true 
	// Deviation = 1.586914 
	// Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word 
	// Packet length = 255 
	// Modulation format = ASK/OOK 
	// Base frequency = 916.500000 
	// Channel number = 0 
	CC1101::reg_config_t const reg_config[] = {
		{CC1101::REG_IOCFG0, 0x06},
		{CC1101::REG_PKTCTRL0, 0x05},
		{CC1101::REG_FSCTRL1, 0x0E},
		{CC1101::REG_FREQ2, 0x23},
		{CC1101::REG_FREQ1, 0x40},
		{CC1101::REG_FREQ0, 0x00},
		{CC1101::REG_MDMCFG4, 0x2E},
		{CC1101::REG_MDMCFG3, 0x3B},
		{CC1101::REG_MDMCFG2, 0x33},
		{CC1101::REG_MDMCFG1, 0x40},
		{CC1101::REG_DEVIATN, 0x00},
		{CC1101::REG_MCSM0, 0x18},
		{CC1101::REG_FOCCFG, 0x1D},
		{CC1101::REG_BSCFG, 0x1C},
		{CC1101::REG_AGCCTRL2, 0xC7},
		{CC1101::REG_AGCCTRL1, 0x00},
		{CC1101::REG_AGCCTRL0, 0xB0},
		{CC1101::REG_FREND1, 0xB6},
		{CC1101::REG_FREND0, 0x11},
		{CC1101::REG_FSCAL3, 0xEA},
		{CC1101::REG_FSCAL2, 0x2A},
		{CC1101::REG_FSCAL1, 0x00},
		{CC1101::REG_FSCAL0, 0x1F},
		{CC1101::REG_TEST0, 0x09},
	};
	
	if(!iface.init())
		return false;
	
	// Load configuration from the TI RF config utility
	iface.config(reg_config, sizeof(reg_config)/sizeof(*reg_config));
	
	return true;
}


GuardianRF::GuardianRF(CC1101 &iface, gpio_pin_t * gdo0, gpio_pin_t * gdo2) : 
	gdo0(gdo0), gdo2(gdo2), Guardian< CC1101 >(iface)
{
		
}
	