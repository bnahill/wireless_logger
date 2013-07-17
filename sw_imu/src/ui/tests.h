#ifndef __UI_TESTS_H_
#define __UI_TESTS_H_

namespace Tests {
bool flash_create_test_file();
bool flash_erase();
/*!
@brief A basic erase/write/read test for flash memory
 */
bool flash_test();
/*!
 @brief A quick check of how many bad blocks exist
 */
bool flash_bad_block_check();

bool flash_ecc_flexibility_test();

bool fs_format();
bool fs_mount();

};

#endif // __UI_TESTS_H_
