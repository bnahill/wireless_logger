#include "ssd1306.h"

/*!
 @name Template specializations
 @{
 */

template<> uint8_t const SSD1306<4,128>::val_compins = 0x02;
template<> uint8_t const SSD1306<4,128>::val_multiplex = 0x1F;
template<> uint8_t const SSD1306<4,128>::val_precharge = 0xF1;
template<> uint8_t const SSD1306<4,128>::val_contrast = 0x8F;

/*!
 @}
 */