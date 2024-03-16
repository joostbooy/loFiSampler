#include "sampleData.h"
/*
int16_t* SampleData::allocate(const char* name, const char* path, uint8_t num_channels, uint32_t size) {
	if ((available_ram_ < size) || (num_entries >= kMaxEntries)) {
		return nullptr;
	}

	int16_t* ptr = block_tail();
	uint8_t slot = num_entries;
	uint8_t id = id_pool[num_entries];

	id_to_slot[id] = slot;

	entry[slot].data = ptr;
	entry[slot].size = size;
	entry[slot].path.write(path);
	entry[slot].num_channels = num_channels;

	available_ram_ -= size;
	++num_entries;

	return ptr;
}

bool SampleData::remove(uint8_t slot) {
	if (slot >= num_entries) {
		return false;
	}

	--num_entries;
	available_ram_ += entry[slot].size;

	if (slot == num_entries) {
		return true;
	}

	for (size_t i = slot; i < num_entries; ++i) {
		entry_[i] = entry_[i + 1];
	}

	reallign_ram_left(slot);

	return true;
}

bool SampleData::truncate(uint8_t slot, uint32_t start, uint32_t end) {
	if ((slot >= num_entries) || (start >= end) || (start + (end - start) > entry[slot].size)) {
		return false;
	}

	entry[slot].start_total += start;

	uint32_t new_size = end - start;
	int16_t* write_ptr = entry[slot].ptr;
	int16_t* read_ptr = write_ptr + start;

	available_ram_ += (entry[slot].size - new_size);
	entry[slot].size = new_size;

	while (new_size--) {
		*write_ptr++ = *read_ptr++;
	}

	reallign_ram_left(slot + 1);

	return true;
}

bool SampleData::convert_to_mono(uint8_t slot) {
	if ((slot >= num_entries) || (entry[slot].num_channels != 2) || (entry[slot].size < 4)) {
		return false;
	}

	int16_t* write_ptr = entry[slot].ptr;
	int16_t* read_ptr = write_ptr;
	uint32_t new_size = entry[slot].size / 2;

	available_ram_ += (entry[slot].size - new_size);
	entry[slot].size = new_size;
	entry[slot].num_channels = 1;

	while (new_size--) {
		int16_t left = *read_ptr++;
		int16_t right = *read_ptr++;
		*write_ptr++ = (left + right) * 0.5f;
	}

	reallign_ram_left(slot + 1);

	return true;
}

int16_t* SampleData::import(Entry* e) {
	if ((e == nullptr) || (available_ram_ < e->size) || (num_entries >= kMaxEntries) || id_is_valid(e->id)) {
		return nullptr;
	}

	uint8_t slot = num_entries;
	int16_t* ptr = block_tail();

	// remove id from pool & keep current valid id
	for (uint8_t i = num_entries; i < kMaxEntries; ++i) {
		if (id_pool[i] == e->id) {
			id_pool[i] = id_pool[num_entries];
			break;
		}
	}

	id_to_slot[e->id] = slot;

	entry[slot].ptr = ptr;
	entry[slot].size = e->size;
	entry[slot].id = e->id;
	entry[slot].name.write(e->name.read());
	entry[slot].path.write(e->path.read());
	entry[slot].start = e->start;
	entry[slot].end = e->end;
	entry[slot].start_total = e->start_total;
	entry[slot].num_channels = e->num_channels;

	available_ram_ -= e->size;
	++num_entries;

	return ptr;
}
*/
