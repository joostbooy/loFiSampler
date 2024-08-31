#include "sampleAllocator.h"

int16_t* SampleAllocator::allocate(const char* path, uint8_t num_channels, size_t size) {
	if ((available_ram_ < size) || (num_samples_ >= kMaxSamples)) {
		return nullptr;
	}

	int16_t* data = block_tail();
	size_t slot = num_samples_++;

	sample_[slot].set_data(data);
	sample_[slot].set_size(size);
	sample_[slot].set_num_channels(num_channels);
	sample_[slot].set_path(path);

	available_ram_ -= size;

	return data;
}

bool SampleAllocator::remove(size_t slot) {
	if (slot >= num_samples_) {
		return false;
	}

	--num_samples_;
	available_ram_ += sample_[slot].size();

	if (slot == num_samples_) {
		return true;
	}

	for (size_t i = slot; i < num_samples_; ++i) {
		sample_[i] = sample_[i + 1];
	}

	reallign_ram_left(slot);

	return true;
}

bool SampleAllocator::truncate(size_t slot, size_t start, size_t end) {
	if ((slot >= num_samples_) || (start >= end) || (start + (end - start) > sample_[slot].size())) {
		return false;
	}

	size_t new_size = end - start;
	int16_t* write_ptr = sample_[slot].data();
	int16_t* read_ptr = write_ptr + start;

	available_ram_ += (sample_[slot].size() - new_size);
	sample_[slot].set_size(new_size);

	while (new_size--) {
		*write_ptr++ = *read_ptr++;
	}

	reallign_ram_left(slot + 1);

	return true;
}

bool SampleAllocator::convert_to_mono(size_t slot) {
	if ((slot >= num_samples_) || (sample_[slot].num_channels() != 2) || (sample_[slot].size() < 4)) {
		return false;
	}

	int16_t* write_ptr = sample_[slot].data();
	int16_t* read_ptr = write_ptr;
	uint32_t new_size = sample_[slot].size() / 2;

	available_ram_ += (sample_[slot].size() - new_size);
	sample_[slot].set_size(new_size);
	sample_[slot].set_num_channels(1);

	while (new_size--) {
		int16_t left = *read_ptr++;
		int16_t right = *read_ptr++;
		*write_ptr++ = (left + right) * 0.5f;
	}

	reallign_ram_left(slot + 1);

	return true;
}

bool SampleAllocator::duplicate(size_t slot) {
	if (slot >= num_samples_ || available_ram_ < sample_[slot].size()) {
		return false;
	}

	size_t size = sample_[slot].size();
	int16_t *src = sample_[slot].data();
	int16_t *dest = allocate(sample_[slot].path(), sample_[slot].num_channels(), size);

	while (size--) {
		*dest++ = *src++;
	}

	return true;
}
