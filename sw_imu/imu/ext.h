#ifndef __IMU_EXT_H_
#define __IMU_EXT_H_

#include "hal.h"

template <EXTDriver &driver>
class ExtCallback {
public:
	typedef void (*cb_t)(void *);
	ExtCallback(cb_t cb, void * arg) :
		cb(cb), arg(arg){}

	void call(){
		if(cb)
			cb(arg);
	}
	
	static constexpr size_t num_channels = EXT_MAX_CHANNELS;
	
	static void init(){
		ExtCallback ** iter;
		for(iter = channels; iter < channels + num_channels; iter++){
			*iter = nullptr;
		}
	}
	
	static void callback(EXTDriver *, expchannel_t channel){
		if(channels[channel])
			channels[channel]->call();
	}
	
	static ExtCallback * channels[num_channels];
protected:
	cb_t cb;
	void * arg;
};

template <EXTDriver &driver>
ExtCallback<driver> * ExtCallback<driver>::channels[num_channels];

#endif // __IMU_EXT_H_
