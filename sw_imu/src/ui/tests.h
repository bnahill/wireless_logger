#ifndef __UI_TESTS_H_
#define __UI_TESTS_H_

class Tests {
public:
	static bool flash_create_test_file();
	static bool flash_erase();
	static bool fs_test();
	static bool flash_power_test();
	static bool fs_power_test();

	/*!
	@brief A basic erase/write/read test for flash memory
	*/
	static bool flash_test();
	/*!
	@brief A quick check of how many bad blocks exist
	*/
	static bool flash_bad_block_check();

	static bool logging_test();

	static bool flash_ecc_flexibility_test();

	static bool fs_format();
	static bool fs_mount();
};

#endif // __UI_TESTS_H_
