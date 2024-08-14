#ifndef SampleData_h
#define SampleData_h

#include <cstddef>
#include <stdint.h>

class SampleData {

public:

	static const size_t kMaxEntries = 128;
	static const size_t kMaxPathLength = 64;

	struct Entry {
		size_t size;	// in samples
		uint8_t num_channels;
		int16_t *data;
		char path[kMaxPathLength];
	};

	void init() {
		num_entries_ = 0;
		available_ram_ = kTotalRam;

		for (uint8_t i = 0; i < kMaxEntries; ++i) {
			entry_[i].data = nullptr;
			entry_[i].size = 0;
		}
	}

	// name
	const char *name(size_t index) {
		return nullptr;//StringUtils::get_file_name_from_path(entry_[index].path);
	}

	uint8_t num_of_entries() {
		return num_entries_;
	}

	Entry *enty(size_t index) {
		return &entry_[index];
	}

	size_t total_ram() {
		return kTotalRam;
	}

	size_t available_ram() {
		return available_ram_;
	}
/*
	bool remove(uint8_t slot);
	bool truncate(uint8_t slot, uint32_t start, uint32_t end);
	bool convert_to_mono(uint8_t slot);

	void save();
	void load();
*/

private:
	const size_t kTotalRam = 0;//sdram.size_bytes() / 2;		// bytes to 16 bit samples
	int16_t* buffer_ = nullptr;//static_cast<int16_t*>(sdram.pointer());
	size_t available_ram_;
	size_t num_entries_ = 0;
	Entry entry_[kMaxEntries];


	int16_t* block_tail() {
		if (num_entries_ == 0) {
			return &buffer_[0];
		} else {
			return entry_[num_entries_ - 1].data + entry_[num_entries_ - 1].size;
		}
	}

	void reallign_ram_left(uint8_t slot) {
		if (slot >= num_entries_) {
			return;
		}

		uint32_t size;
		int16_t* write_ptr;
		int16_t* read_ptr = entry_[slot].data;

		if (slot == 0) {
			write_ptr = &buffer_[0];
		} else {
			write_ptr = entry_[slot - 1].data + entry_[slot - 1].size;
		}

		if (write_ptr >= read_ptr) {
			return;
		}

		for (uint8_t i = slot; i < num_entries_; ++i) {
			entry_[i].data = write_ptr;
			size = entry_[i].size;
			while (size--) {
				*write_ptr++ = *read_ptr++;
			}
		}
	}

};

#endif
