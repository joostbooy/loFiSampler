#ifndef SampleAllocator_h
#define SampleAllocator_h

#include <cstddef>
#include <stdint.h>

#include "sdram.h"
#include "stringUtils.h"
#include "fileReader.h"
#include "fileWriter.h"
#include "sample.h"
//#include "wavImporter.h"

class SampleAllocator {

public:

	static const size_t kMaxEntries = 128;
	static const size_t kMaxPathLength = 64;

	void init(Sdram *sdram, Sample *sample) {
		sample_ = sample;
		buffer_ = sdram->pointer();
		max_ram_ = sdram->size_bytes() / 2; // bytes to 16 bit samples
		available_ram_ = max_ram_;

		num_samples_ = 0;

	//	for (uint8_t i = 0; i < kMaxEntries; ++i) {
	//		sample_[i].data = nullptr;
	//		sample_[i].size = 0;
	//		sample_[i].path[0] = '\0';
	//	}
	}

	uint8_t num_samples() {
		return num_samples_;
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

	void load(FileReader &fileReader) {
		//	fileReader.read(num_samples_);
		//	fileReader.read(available_ram_);
		//	fileReader.read(name_);
		//	fileReader.read(sample_);

		for (size_t i = 0; i < kMaxEntries; ++i) {
			// wavImporter_->import(sample_[i].path, sample_[i].num_channels == 1);
		}
	}

private:
	//WavImporter *wavImporter_;
	Sample *sample_;
	int16_t *buffer_;
	size_t max_ram_;
	size_t available_ram_;
	size_t num_samples_ = 0;
	static const size_t kMaxSamples = 128;

	int16_t* block_tail() {
		if (num_samples_ == 0) {
			return &buffer_[0];
		} else {
			return sample_[num_samples_ - 1].data() + sample_[num_samples_ - 1].size();
		}
	}

	void reallign_ram_left(uint8_t slot) {
		if (slot >= num_samples_) {
			return;
		}

		uint32_t size;
		int16_t* write_ptr;
		int16_t* read_ptr = sample_[slot].data();

		if (slot == 0) {
			write_ptr = &buffer_[0];
		} else {
			write_ptr = sample_[slot - 1].data() + sample_[slot - 1].size();
		}

		if (write_ptr >= read_ptr) {
			return;
		}

		for (uint8_t i = slot; i < num_samples_; ++i) {
			sample_[i].set_data(write_ptr);
			size = sample_[i].size();
			while (size--) {
				*write_ptr++ = *read_ptr++;
			}
		}
	}

};

#endif
