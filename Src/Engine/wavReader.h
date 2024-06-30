#ifndef WavReader_h
#define WavReader_h

#include "stmf4lib.h"
#include "file.h"
#include "wavFile.h"


class WavReader {

public:

	enum Error {
		SD_ERROR,
		NO_RIFF,
		NOT_WAV,
		BAD_WAV_FORMAT,
		PARSE_CHUNK_FAIL,
		WAV_OK
	};

	const char* error_text(Error error) {
		switch (error)
		{
		case SD_ERROR:				return "SD ERROR";
		case NO_RIFF:				return "NO RIFF";
		case NOT_WAV:				return "NOT A WAV FILE";
		case UNSUPPORTED_FORMAT:	return "UNSUPPORTED FORMAT";
		case UNSUPPORTED_BIT_DEPTH:	return "UNSUPPORTED BIT DEPTH";
		case UNSUPPORTED_CHANNELS:	return "UNSUPPORTED CHANNELS";
		case PARSE_CHUNK_FAIL:		return "PARSE CHUNK FAIL";
		case WAV_OK:				return "WAV OK";
		default:
			break;
		}
		return "UNKNOW";
	}

	void init(WavFile *wavFile) {
		wavFile_ = wavFile;
	}

	Error read(File *file) {
		Error error;
		uint32_t chunk_id;
		uint32_t chunk_size;
		file_ = file;

		chunk_id = read_u32();
		if (chunk_id != RIFF_CHUNK_ID){
			return NO_RIFF;
		}

		//discard chunk size
		file_->advance(4);

		wavFile_->riff.format = read_u32();
		if (wavFile_->riff.format != RIFF_FORMAT_WAVE) {
			return NOT_WAV;
		}

		while (chunk_id != DATA_CHUNK_ID) {
			if (file_->end()) {
				return PARSE_CHUNK_FAIL;
			}

			chunk_id = read_u32();
			chunk_size = read_u32();
			error = parse_chunk(chunk_id, chunk_size);

			if (error != WAV_OK) {
				return error;
			}
		}

		return WAV_OK;
	}

private:
	File *file_;
	WavFile* wavFile_;

	static const uint32_t RIFF_CHUNK_ID		= 0x46464952;
	static const uint32_t FMT_CHUNK_ID		= 0x20746D66;
	static const uint32_t DATA_CHUNK_ID		= 0x61746164;
	static const uint32_t RIFF_FORMAT_WAVE	= 0x45564157;
	static const uint16_t WAVE_FORMAT_PCM	= 0x0001;

	Error parse_chunk(uint32_t chunk_id, uint32_t chunk_size) {
		switch (chunk_id)
		{
		case FMT_CHUNK_ID:
			if (file_->read(&wavFile_->format, 16) == false) {
				return SD_ERROR;
			}

			if (chunk_size > 16) {
				file_->advance(chunk_size - 16);
			}

			if (wavFile_->format.audio_format != WAVE_FORMAT_PCM) {
				return UNSUPPORTED_FORMAT;
			}

			if (wavFile_->format.bit_depth < 16) {
				return UNSUPPORTED_BIT_DEPTH;
			}

			if ((wavFile_->format.num_of_channels < 1) || (wavFile_->format.num_of_channels > 2)) {
				return UNSUPPORTED_CHANNELS;
			}
			break;
		case DATA_CHUNK_ID:
			wavFile_->data.size = chunk_size;
			wavFile_->data.offset = file_->position();
			break;
		default:
			file_->advance(chunk_size);
			break;
		}
		return WAV_OK;
	}

	uint32_t read_u32() {
		UINT br;
		uint8_t data[4];

		sd_ok_= file_->read(&data[0], 4);

		return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	};

};

#endif
