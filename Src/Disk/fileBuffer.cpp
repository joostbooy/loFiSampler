#include "FileBuffer.h"

uint8_t FileBuffer::data_[kSize] __attribute__((section(".dtcm")));
