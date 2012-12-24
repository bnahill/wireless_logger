#ifndef __TOOLCHAIN_H_
#define __TOOLCHAIN_H_

#ifdef __CC_ARM
	#define RESTRICT
#elif defined ( __GNUC__ )
	#define RESTRICT restrict
#endif

#endif
