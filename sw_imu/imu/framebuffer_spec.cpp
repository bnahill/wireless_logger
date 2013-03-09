/*!
 @file framebuffer_spec.cpp
 @brief Template specializations for \ref FrameBuffer.
 @note This has been phased out I think, but it's still here for the next few
 revisions anyways.
 @author Ben Nahill <bnahill@gmail.com>
 */

#include "imu/graphics/comicsans4.h"
#include "imu/graphics/courier3.h"
#include "imu/graphics/mediumfont.h"
#include "imu/graphics/smallfont.h"

#include "imu/framebuffer.cpp"

// template uint32_t FrameBuffer<4, 128>::write_text_centered<Courier3>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);
// template uint32_t FrameBuffer<4, 128>::write_text_centered<ComicSans4>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);
// template uint32_t FrameBuffer<4, 128>::write_text_centered<MediumFont>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);
// template uint32_t FrameBuffer<4, 128>::write_text_centered<SmallFont>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);
/*
template uint32_t FrameBuffer<4, 128>::write_text<SmallFont>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);
template uint32_t FrameBuffer<4, 128>::write_text<MediumFont>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);
template uint32_t FrameBuffer<4, 128>::write_text<Courier3>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);
template uint32_t FrameBuffer<4, 128>::write_text<ComicSans4>(char const * text, uint8_t page, uint8_t center_column, uint32_t max_width);*/