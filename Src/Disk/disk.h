#ifndef Disk_h
#define Disk_h

#include "sdmmc.h"
#include "ff.h"
#include "directory.h"
#include "entry.h"
#include "file.h"
#include "diskio.h"

class Disk {

public:

	static Sdmmc *sdmmc_;

	Directory &dir() {
		return dir_;
	}

	Entry &entry() {
		return entry_;
	}

	File &file() {
		return file_;
	}

	void init(Sdmmc *sdmmc) {
		sdmmc_ = sdmmc;

		const int root = 0;

		dir_.init(&fdir, root, &result);
		entry_.init(&fdir, &fil_info, dir_.path_ptr());
		file_.init(&fil, dir_.path_ptr(), &result);

		mount();
	}

	bool mount() {
		if (!mounted()) {
			return f_mount(&fs, dir_.root(), 1) == FR_OK;
		} else {
			return true;
		}
	}

	bool mounted() {
		return sdmmc_->initialised();
	}

	FRESULT last_result() {
		return result;
	}

	void reset() {
		dir_.reset();
		dir_.close();
		entry_.reset();
		file_.close();
	}

	void available_kb(uint32_t *total_blocks, uint32_t *free_blocks) {
		FATFS* fs_ptr = &fs;
		uint32_t free_clusters;

		result = f_getfree(dir_.root(), &free_clusters, &fs_ptr);
		if (result == FR_OK) {
			*total_blocks = ((fs_ptr->n_fatent - 2) * fs_ptr->csize) / 2;
			*free_blocks = (free_clusters * fs_ptr->csize) / 2;
		}
	}

private:
	FATFS fs;
	DIR fdir;
	FIL fil;
	FILINFO fil_info;
	FRESULT result;

	Directory dir_;
	Entry entry_;
	File file_;
};

#endif
