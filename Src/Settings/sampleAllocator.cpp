#include "sampleAllocator.h"

int16_t* SampleAllocator::allocate(const char* path, uint8_t num_channels, size_t size) {
	if ((available_ram_ < size) || (num_samples() >= kMaxSamples) || (path_excists(path) == true)) {
		return nullptr;
	}

	int16_t* data = buffer_tail();

	for (size_t i = 0; i < kMaxSamples; ++i) {
		if (!sample_[i].has_data()) {
			sampleList_.push(&sample_[i]);

			sample_[i].init();
			sample_[i].set_data(data);
			sample_[i].set_num_channels(num_channels);
			sample_[i].set_size(size);
			sample_[i].set_end(size);
			sample_[i].set_loop_end(size);
			sample_[i].set_path(path);

			available_ram_ -= size;

			return data;
		}
	}

	return nullptr;
}

bool SampleAllocator::remove(size_t index) {
	Sample *sample = read_list(index);

	if (index >= num_samples()) {
		return false;
	}

	available_ram_ += sample->size();
	sample->init();
	sampleList_.remove_by_index(index);
	reallign_ram_left(index);

	return true;
}

bool SampleAllocator::truncate(size_t index, size_t start, size_t end) {
	Sample *sample = read_list(index);

	if ((index >= num_samples()) || (start >= end) || (start + (end - start) > sample->size())) {
		return false;
	}

	size_t new_size = end - start;
	int16_t* write_ptr = sample->data();
	int16_t* read_ptr = write_ptr + start;

	available_ram_ += (sample->size() - new_size);
	sample->set_size(new_size);
	sample->refresh_start_end_points();

	while (new_size--) {
		*write_ptr++ = *read_ptr++;
	}

	reallign_ram_left(index + 1);

	return true;
}

bool SampleAllocator::convert_to_mono(size_t index) {
	Sample *sample = read_list(index);

	if ((index >= num_samples()) || (sample->num_channels() != 2) || (sample->size() < 4)) {
		return false;
	}

	int16_t* write_ptr = sample->data();
	int16_t* read_ptr = write_ptr;
	uint32_t new_size = sample->size() / 2;

	available_ram_ += (sample->size() - new_size);
	sample->set_size(new_size);
	sample->set_num_channels(1);
	sample->refresh_start_end_points();

	while (new_size--) {
		int16_t left = *read_ptr++;
		int16_t right = *read_ptr++;
		*write_ptr++ = (left + right) * 0.5f;
	}

	reallign_ram_left(index + 1);

	return true;
}
