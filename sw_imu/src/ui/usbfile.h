#ifndef __UI_USBFILE_H_
#define __UI_USBFILE_H_

#include "platform.h"
#include "acquisition.h"
#include "ui/ui.h"


//! @addtogroup USBFile
//! @{

class USBFile{
public:
	USBFile(char const * name, char const * format) :
		name(name), format(format)
	{}
	
protected:
	//! Some sort of descriptive name
	char const * name;
	
	//! A string indicating the format and version number
	char const * format;
	
	//! Query the total size in bytes
	virtual size_t get_size() = 0;
	//! Seek to the beginning
	virtual void seek_reset() = 0;
	/*!
	 @brief Read up to max_bytes bytes
	 @param dst The destination buffer
	 @param max_bytes The size of dst
	 @return The number of bytes copied
	 
	 @note max_bytes must be larger than the minimum block size for the format
	 */
	virtual size_t read_file(uint8_t *dst, size_t max_bytes) = 0;
	
	//! Let this be used by USBTerm to mange the linked list
	USBFile *next;
	friend class USBTerm;
};

class USBStaticBinary : public USBFile {
public:
	USBStaticBinary(char const * name, char const * format,
	                uint8_t const * data, size_t size) :
		USBFile(name, format), position(0), data(data), size(size) {}
protected:
	size_t position;
	size_t size;
	uint8_t const * data;
	
	size_t get_size(){return size;}
	void seek_reset(){position = 0;}
	size_t read_file(uint8_t *dst, size_t max_bytes){
		uint8_t const * iter;
		max_bytes = min(max_bytes, size - position);
		for(iter = data + position; iter < (data + position + max_bytes); iter++){
			*dst++ = *iter;
		}
		position += max_bytes;
		return max_bytes;
	}
};

//! @}

#endif // __UI_USBFILE_H_