#ifndef __UI_TESTS_H_
#define __UI_TESTS_H_

namespace Tests {

bool flash_format();
/*!
 @brief A basic erase/write/read test for flash memory
 */
bool flash_test();
/*!
 @brief A quick check of how many bad blocks exist
 */
bool flash_bad_block_check();

};

#endif // __UI_TESTS_H_
