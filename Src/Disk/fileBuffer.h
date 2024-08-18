#ifndef FileBuffer_h
#define FileBuffer_h

#include <stdint.h>
#include <cstddef>

class FileBuffer {

public:

	static size_t size() {
		return kSize;
	}

	static uint8_t *data(size_t index = 0) {
		return &data_[index];
	}

private:
	static constexpr size_t kSize = 512;
	static uint8_t data_[kSize] __attribute__((section(".dtcm")));
};
#endif
