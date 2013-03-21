/*!
 @file guardian_spec.cpp
 @brief Template specializations for using \ref Guardian with \ref CC1101
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "guardian.h"

template <>
bool Guardian<CC1101>::init(){
	CC1101::status_t status;
	CC1101::state_t state;
	
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
	CC1101::reg_config_t constexpr reg_config[] = {
		{CC1101::REG_IOCFG0, 0x06},
		//{CC1101::REG_PKTCTRL0, 0x05},
		{CC1101::REG_PKTCTRL1, 0x00},
		// Use direct mode, just output data as it comes
		// This commented out to avoid overvlow potential
		{CC1101::REG_PKTCTRL0, 0x12},
		{CC1101::REG_FSCTRL1, 0x06},
		{CC1101::REG_FSCTRL0, 0x00},
		{CC1101::REG_FREQ2, 0x23},
		{CC1101::REG_FREQ1, 0x40},
		{CC1101::REG_FREQ0, 0x00},
		{CC1101::REG_CHANNR, 0x00},
		
		{CC1101::REG_MDMCFG4, 0xC8},
		{CC1101::REG_MDMCFG3, 0x4A},
		{CC1101::REG_MDMCFG2, 0x30},
		{CC1101::REG_MDMCFG1, 0x42},
		{CC1101::REG_MDMCFG0, 0xF8},
// 		{CC1101::REG_MDMCFG4, 0x2E},
// 		{CC1101::REG_MDMCFG3, 0x3B},
// 		{CC1101::REG_MDMCFG2, 0x33},
// 		{CC1101::REG_MDMCFG1, 0x40},
		{CC1101::REG_DEVIATN, 0x00},
		{CC1101::REG_MCSM2, 0x07},
		{CC1101::REG_MCSM1, 0x30},
		{CC1101::REG_MCSM0, 0x18},
		{CC1101::REG_FOCCFG, 0x1D},
		{CC1101::REG_BSCFG, 0x6C},
		{CC1101::REG_AGCCTRL2, 0xA4},
		{CC1101::REG_AGCCTRL1, 0x50},
		{CC1101::REG_AGCCTRL0, 0x92},
		{CC1101::REG_FREND1, 0x56},
		{CC1101::REG_FREND0, 0x11},
		{CC1101::REG_FSCAL3, 0xE9},
		{CC1101::REG_FSCAL2, 0x2A},
		{CC1101::REG_FSCAL1, 0x00},
		{CC1101::REG_FSCAL0, 0x1F},
		{CC1101::REG_TEST2, 0x81},
		{CC1101::REG_TEST1, 0x35},
		//{CC1101::REG_TEST0, 0x09},
	};
	
	if(!iface.init())
		return false;
	
	// Load configuration from the TI RF config utility
	iface.config(reg_config, sizeof(reg_config)/sizeof(*reg_config));
	
	// Configure GDO pins to output serial data
	iface.set_gdo_mode(CC1101::GDO0, 0x0C); // Data out
	iface.set_gdo_mode(CC1101::GDO2, 0x0B); // Clock out
	
	iface.strobe(CC1101::CMD_SIDLE);
	
	// Clear FIFOs
	iface.strobe(CC1101::CMD_SFRX);
	iface.strobe(CC1101::CMD_SFTX);
	
// 	for(i = 0; i < 50; i++){
// 		iface.strobe(CC1101::CMD_SRX);
// 		status = iface.get_status();
// 		state = iface.get_state(status);
// 		if(state == CC1101::ST_RX)
// 			break;
// 		chThdSleep(1);
// 	} if(i == 50){
// 		return false;
// 	}

	iface.write_reg(CC1101::REG_PKTCTRL0, 0x12);
	if(iface.read_reg(CC1101::REG_PKTCTRL0) != 0x12){
		return false;
	}
	
	iface.strobe(CC1101::CMD_SRX);


	return true;
}


GuardianRF::GuardianRF(CC1101 &iface, gpio_pin_t * gdo0, gpio_pin_t * gdo2) : 
	gdo0(gdo0), gdo2(gdo2), Guardian< CC1101 >(iface)
{
	
}
