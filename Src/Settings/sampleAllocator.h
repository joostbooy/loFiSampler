#ifndef SampleAllocator_h
#define SampleAllocator_h

#include <cstddef>
#include <stdint.h>

#include "sdram.h"
#include "stringUtils.h"
#include "fileReader.h"
#include "fileWriter.h"
#include "sample.h"
#include "stack.h"

class SampleAllocator {

public:

	static const size_t kMaxSamples = 128;
	static const size_t kMaxPathLength = 64;

	void init(Sdram *sdram, Sample *sample) {
		sample_ = sample;
		buffer_ = sdram->pointer();
		max_ram_ = sdram->size_bytes() / 2; // bytes to 16 bit samples
		available_ram_ = max_ram_;
		sampleMap_.clear();
	}

	size_t num_samples() {
		return sampleMap_.size();
	}

	Sample* read_map(size_t index) {
		return sampleMap_.read(index);
	}

	size_t available_ram() {
		return available_ram_;
	}

	size_t max_ram() {
		return max_ram_;
	}

	int16_t* allocate(const char* path, uint8_t num_channels, size_t size);
	bool remove(size_t slot);
	bool truncate(size_t slot, size_t start, size_t end);
	bool convert_to_mono(size_t slot);
	bool duplicate(size_t slot);

private:
	Sample *sample_;
	int16_t *buffer_;
	size_t max_ram_;
	size_t available_ram_;
	Stack<Sample*, kMaxSamples>sampleMap_;

	int16_t* block_tail() {
		if (num_samples() == 0) {
			return &buffer_[0];
		} else {
			Sample *sample = read_map(num_samples() - 1);
			return sample->data() + sample->size();
		}
	}

	void reallign_ram_left(size_t index) {
		if (index >= num_samples()) {
			return;
		}

		size_t size;
		int16_t* write_ptr;
		int16_t* read_ptr = read_map(index)->data();

		if (index == 0) {
			write_ptr = &buffer_[0];
		} else {
			Sample *sample = read_map(index - 1);
			write_ptr = sample->data() + sample->size();
		}

		if (write_ptr >= read_ptr) {
			return;
		}

		for (size_t i = index; i < num_samples(); ++i) {
			Sample *sample = read_map(i);

			sample->set_data(write_ptr);
			size = sample->size();
			while (size--) {
				*write_ptr++ = *read_ptr++;
			}
		}
	}

};

#endif
