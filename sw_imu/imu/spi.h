#ifndef __IMU_SPI_H_
#define __IMU_SPI_H_

#include "ch.h"
#include "hal.h"

class SPI {
public:
	typedef SPIConfig slave_config_t;
	
	SPI(SPIDriver &driver) :
		driver(driver)
	{
		spiObjectInit(&driver);
	}
	
	void exchange(slave_config_t &config, size_t n, void const * tx_buff, void * rx_buff){
		spiAcquireBus(&driver);
		spiStart(&driver, &config);
		spiSelect(&driver);
		spiExchange(&driver, n, tx_buff, rx_buff);
		spiUnselect(&driver);
		//spiStop(&driver);
		spiReleaseBus(&driver);
	}
	
protected:
	SPIDriver &driver;
};

#endif // __IMU_SPI_H_
