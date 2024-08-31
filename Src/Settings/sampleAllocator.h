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

	static const size_t kMaxEntries = 128;
	static const size_t kMaxPathLength = 64;

	void init(Sdram *sdram, Sample *sample) {
		sample_ = sample;
		buffer_ = sdram->pointer();
		max_ram_ = sdram->size_bytes() / 2; // bytes to 16 bit samples
		available_ram_ = max_ram_;
		stack_.clear();
	}

	uint8_t num_samples() {
		return stack_.size();
	}

	Sample* read_list(size_t index) {
		return stack_.read(index);
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
	static const size_t kMaxSamples = 128;
	Stack<Sample*, kMaxSamples>stack_;

	int16_t* block_tail() {
		if (stack_.size() == 0) {
			return &buffer_[0];
		} else {
			Sample *sample = stack_.read(stack_.size() - 1);
			return sample->data() + sample->size();
		}
	}

	void reallign_ram_left(size_t index) {
		if (index >= stack_.size()) {
			return;
		}

		size_t size;
		int16_t* write_ptr;
		int16_t* read_ptr = stack_.read(index)->data();

		if (index == 0) {
			write_ptr = &buffer_[0];
		} else {
			Sample *sample = stack_.read(index - 1);
			write_ptr = sample->data() + sample->size();
		}

		if (write_ptr >= read_ptr) {
			return;
		}

		for (size_t i = index; i < stack_.size(); ++i) {
			Sample *sample = stack_.read(i);

			sample->set_data(write_ptr);
			size = sample->size();
			while (size--) {
				*write_ptr++ = *read_ptr++;
			}
		}
	}

};

#endif
