#include "tests.h"
#include "platform/platform.h"
#include "ui/ui.h"
#include "stdlib.h"
#include "string.h"
#include "coffee/cfs-coffee.h"
#include "flogfs.h"

using namespace Platform;

bool Tests::fs_format() {
	bool res;
	oled.fb.clear_area(1);

	oled.fb.write_text_centered<SmallFont>("Formatting...", 2);
	oled.update();
	res = (flogfs_format() == FLOG_SUCCESS);

	oled.fb.clear_area(1);
	if(res){
		oled.fb.write_text_centered<SmallFont>("Success", 2);
	} else {
		oled.fb.write_text_centered<SmallFont>("Failed", 2);
	}
	oled.update();
	UI::wait_for_button(UI::MASK_SELECT);
	return res;
}


bool Tests::fs_mount(){
	bool res;
	oled.fb.clear_area(1);

	oled.fb.write_text_centered<SmallFont>("Mounting...", 2);
	oled.update();
	res = (flogfs_mount() == FLOG_SUCCESS);

	oled.fb.clear_area(1);
	if(res){
		oled.fb.write_text_centered<SmallFont>("Success", 2);
	} else {
		oled.fb.write_text_centered<SmallFont>("Failed", 2);
	}
	oled.update();
	UI::wait_for_button(UI::MASK_SELECT);
	return res;
}

bool Tests::flash_erase(){
	oled.fb.clear_area(1);

	oled.fb.write_text_centered<SmallFont>("Erasing...", 2);
	for(auto i = flash.num_blocks - 1; i; i--){
		flash.erase_block(i);
	}
	oled.fb.clear_area(1);
	oled.fb.write_text_centered<SmallFont>("Done", 2);
	UI::wait_for_button(UI::MASK_SELECT);
	return true;
}

bool Tests::flash_ecc_flexibility_test() {
	uint32_t constexpr block = 11;
	uint32_t constexpr page = 0;
	uint32_t constexpr bytes = 2112;
	
	char textbuffer[32];
	
	uint8_t buffer[bytes];
	
	oled.fb.clear_area(1);

	oled.fb.write_text_centered<SmallFont>("Starting", 2);
	oled.update();

	textbuffer[0] = 0;
	
	for(uint32_t i = 0; i < bytes; i++){
		buffer[i] = i;
	}
	
	flash.lock();
	flash.erase_block(block);
	
	// Write to protected areas
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer, 0, 512);
	flash.page_write_continued(buffer, 0x804, 4);
	flash.page_commit();
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer + 0x200, 0x200, 512);
	flash.page_write_continued(buffer, 0x814, 4);
	flash.page_commit();
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer + 0x400, 0x400, 512);
	flash.page_write_continued(buffer, 0x824, 4);
	flash.page_commit();
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer + 0x600, 0x600, 512);
	flash.page_write_continued(buffer, 0x834, 4);
	flash.page_commit();
	
	// Write to unprotected areas
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer, 0x802, 2);
	flash.page_commit();
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer, 0x812, 2);
	flash.page_commit();
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer, 0x822, 2);
	flash.page_commit();
	
	flash.page_open(block, page);
	flash.page_write_continued(buffer, 0x832, 2);
	flash.page_commit();
	
	flash.unlock();
	
	// Check protected areas
	
	if(!flash.read_page(buffer, block, page, 0, 2048)){
		imu_sprint(textbuffer, "Failed to read\nmain!");
		goto end;
	}
	
	if(!flash.read_page(buffer, block, page, 0, 2048)){
		imu_sprint(textbuffer, "Failed to read\nmain twice!");
		goto end;
	}
	
	
	
	for(uint32_t i = 0; i < 2048; i++){
		if(buffer[i] != static_cast<uint8_t>(i)){
			imu_sprint(textbuffer, "Error at ", i);
			goto end;
		}
	}
	
	if(!flash.read_page(buffer, block, page, 0x802, 2)){
		imu_sprint(textbuffer, "Failed to read\nspare 0!");
		goto end;
	}
	
	for(uint32_t i = 0; i < 2; i++){
		if(buffer[i] != static_cast<uint8_t>(i)){
			imu_sprint(textbuffer, "Error spare 0\nat ", i);
			goto end;
		}
	}
	
	if(!flash.read_page(buffer, block, page, 0x812, 2)){
		imu_sprint(textbuffer, "Failed to read\nspare 1!");
		goto end;
	}
	
	for(uint32_t i = 0; i < 2; i++){
		if(buffer[i] != static_cast<uint8_t>(i)){
			imu_sprint(textbuffer, "Error spare 1\nat ", i);
			goto end;
		}
	}
	
	if(!flash.read_page(buffer, block, page, 0x822, 2)){
		imu_sprint(textbuffer, "Failed to read\nspare 2!");
		goto end;
	}
	
	for(uint32_t i = 0; i < 2; i++){
		if(buffer[i] != static_cast<uint8_t>(i)){
			imu_sprint(textbuffer, "Error spare 2\nat ", i);
			goto end;
		}
	}
	
	if(!flash.read_page(buffer, block, page, 0x832, 2)){
		imu_sprint(textbuffer, "Failed to read\nspare 3!");
		goto end;
	}

	for(uint32_t i = 0; i < 2; i++){
		if(buffer[i] != static_cast<uint8_t>(i)){
			imu_sprint(textbuffer, "Error spare 3\nat ", i);
			goto end;
		}
	}
	
	imu_sprint(textbuffer, "Success!");

end:
	
	
	oled.fb.write_text_centered<SmallFont>(textbuffer, 2);
	
	UI::wait_for_button(UI::MASK_SELECT);
	
	return true;
}


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

