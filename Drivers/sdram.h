#ifndef Sdram_h
#define Sdram_h

#include "stm32f7xx.h"

class Sdram {

public:

	void init();

	constexpr const size_t size_bytes() {
		return kSize;
	}

	constexpr int16_t* pointer() {
		return reinterpret_cast<int16_t*>(kSdram_adress);
	}

private:
	static const size_t kSize = 64000000;
	static const uint32_t kSdram_adress = 0xC0000000;
};

#endif
