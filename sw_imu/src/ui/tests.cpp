#include "tests.h"
#include "platform/platform.h"
#include "ui/ui.h"
#include "stdlib.h"
#include "string.h"
#include "coffee/cfs-coffee.h"

using namespace Platform;

bool Tests::flash_create_test_file(){
	int fd;
	constexpr auto size = 2048;
	char testbuffer[size], readbuffer[size];
	oled.fb.clear_area(1);
	oled.fb.write_text<SmallFont>("Writing...", 1, 0);
	oled.update();
	fd = cfs_open("testfile", CFS_WRITE);
	if(-1 == fd){
		oled.fb.write_text<SmallFont>("Error opening file", 2, 0);
		goto error;
	}
	for(uint32_t i = 0; i < size; i++){
		testbuffer[i] = i & 77;
	}
	if(cfs_write(fd, testbuffer, size) != size){
		oled.fb.write_text<SmallFont>("Error writing file", 2, 0);
		goto error;
	}
	
	cfs_close(fd);
	
	
	
	oled.fb.clear_area(1);
	oled.fb.write_text<SmallFont>("Reading...", 1, 0);
	oled.update();
	fd = cfs_open("testfile", CFS_READ);
	if(-1 == fd){
		oled.fb.write_text<SmallFont>("Error opening file", 2, 0);
		goto error;
	}
	if(cfs_read(fd, readbuffer, size) != size){
		oled.fb.write_text<SmallFont>("Error reading file", 2, 0);
		goto error;
	}
	cfs_close(fd);
	if(memcmp(testbuffer, readbuffer, size) != 0){
		oled.fb.write_text<SmallFont>("RW data doesn't match", 2, 0);
		goto error;
	}
	oled.fb.write_text<SmallFont>("Tests passed!", 2, 0);
	oled.update();
	UI::wait_for_button(UI::MASK_SELECT);
	return true;
error:
	oled.update();
	UI::wait_for_button(UI::MASK_SELECT);
	return false;
}

bool Tests::flash_bad_block_check(){
	uint32_t bad_block_count;
	char text[32] = "Bad blocks:";
	oled.fb.clear_area(1);
	
	oled.fb.write_text<SmallFont>("Checking bad blocks...", 1, 0);
	oled.update();
	
	bad_block_count = 0;
	for(uint16_t i = 0; i < flash.num_blocks; i++){
		if(flash.check_block_bad(i)){
			bad_block_count += 1;
		}
	}
	
	uint_to_string(bad_block_count, text+11);
	oled.fb.write_text<SmallFont>(text, 2, 0);
	oled.update();
	
	UI::wait_for_button(UI::MASK_SELECT);

	return true;
}

bool Tests::flash_test(){
	MT29FxG01::page_t * write_buff;
	MT29FxG01::page_t * read_buff;
	uint32_t i;
	
	constexpr uint16_t block = 44;
	constexpr uint16_t page = 0;
	
	bool err = false;
	
	oled.fb.clear_area(1);
	
	write_buff = reinterpret_cast<MT29FxG01::page_t *>(chPoolAlloc(&flash.pool));
	read_buff = reinterpret_cast<MT29FxG01::page_t *>(chPoolAlloc(&flash.pool));
	
	if(write_buff){
		oled.fb.write_text<SmallFont>("Allocated 1", 1, 0);
	} else {
		oled.fb.write_text<SmallFont>("Didn't allocate", 1, 0);
		err = true;
	}
	
	if(read_buff){
		oled.fb.write_text<SmallFont>("Allocated 2", 2, 0);
	} else {
		oled.fb.write_text<SmallFont>("Didn't allocate", 2, 0);
		err = true;
	}
	
	oled.update();
	
	
	if(err){
		goto dont_run;
	}
	
	
	chThdSleep(MS2ST(2000));
	
	
	oled.fb.clear_area(1);
	oled.fb.write_text<SmallFont>("Erasing block", 1, 0);
	oled.update();
	if(!flash.erase_block(block)){
		oled.fb.write_text<SmallFont>("Failure reported", 2, 0);
		err = true;
	} else {
		oled.fb.write_text<SmallFont>("Success reported", 2, 0);
	}
	oled.update();
	if(err) goto dont_run;
	
	
	chThdSleep(MS2ST(2000));
	Random::init();
	for(i = 0; i < flash.page_size; i++){
		(static_cast<uint8_t *>(*write_buff))[i] = (uint8_t)Random::get_value();
	}
	Random::close();
		
	oled.fb.clear_area(1);
	oled.fb.write_text<SmallFont>("Write random block", 1, 0);
	oled.update();
	if(flash.write_page(*write_buff, block, page)){
		oled.fb.write_text<SmallFont>("Success reported", 2, 0);
	} else {
		oled.fb.write_text<SmallFont>("Failure reported", 2, 0);
		err = true;
	}
	oled.update();
	
	if(err) goto dont_run;
	
	chThdSleep(MS2ST(2000));
	
	oled.fb.clear_area(1);
	oled.fb.write_text<SmallFont>("Read back block", 1, 0);
	oled.update();
	if(flash.read_page(reinterpret_cast<uint8_t *>(*read_buff), block, page, 0, flash.page_size)){
		oled.fb.write_text<SmallFont>("Success reported", 2, 0);
	} else {
		oled.fb.write_text<SmallFont>("Failure reported", 2, 0);
	}
	oled.update();
	
	chThdSleep(MS2ST(2000));
	
	oled.fb.clear_area(1);
	oled.fb.write_text<SmallFont>("Comparing", 1, 0);
	oled.update();
	
	for(i = 0; i < flash.page_size; i++){
		if(reinterpret_cast<uint8_t *>(*write_buff)[i] != reinterpret_cast<uint8_t *>(*read_buff)[i]){
			oled.fb.write_text<SmallFont>("Mismatch found", 2, 0);
			break;
		}
	} if(i == flash.page_size){
		oled.fb.write_text<SmallFont>("Test passed!", 2, 0);
	}
	
	oled.update();
	
	
	chPoolFree(&flash.pool, write_buff);
	chPoolFree(&flash.pool, read_buff);
	
dont_run:

	
	// Wait for select button
	UI::wait_for_button(UI::MASK_SELECT);
	
	return true;
}

