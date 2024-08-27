#ifndef WaveFile_h
#define WaveFile_h

#include "file.h"
#include "wavFile.h"

class WaveFile {

public:

	enum Result {
		SD_ERROR,
		NOT_RIFF,
		NOT_WAV,
		UNSUPPORTED_WAV_FORMAT,
		PARSE_CHUNK_FAIL,
		WAV_OK
	};

	const char* result_text(Result result_) {
		switch (result_)
		{
		case SD_ERROR:					return "SD ERROR";
		case NOT_RIFF:					return "NOT RIFF";
		case NOT_WAV:					return "NOT WAV";
		case UNSUPPORTED_WAV_FORMAT:	return "UNSUPPORTED WAV FORMAT";
		case PARSE_CHUNK_FAIL:			return "PARSE CHUNK FAIL";
		case WAV_OK:					return "WAV OK";
		default:
			break;
		}
		return "UNKNOW";
	}

	struct Data_chunk {
		uint32_t size;
		uint32_t offset;
	}data_chunk;

	struct Riff_chunk {
		uint32_t size;
		uint32_t format;
	}riff_chunk;

	struct Fmt_chunk {
		uint16_t audio_format;
		uint16_t num_of_channels;
		uint32_t sample_rate;
		uint32_t byte_rate;
		uint16_t block_allign;
		uint16_t bit_depth;
	}fmt_chunk;

	Result result() {
		return result_;
	}

	Result parse(File *file) {
		uint32_t chunk_id;
		uint32_t chunk_size;
		file_ = file;

		chunk_id = read_u32();
		if (chunk_id != RIFF_CHUNK_ID){
			return NOT_RIFF;
		}

		riff_chunk.size = read_u32();
		riff_chunk.format = read_u32();
		if (riff_chunk.format != RIFF_FORMAT_WAVE) {
			return NOT_WAV;
		}

		while (chunk_id != DATA_CHUNK_ID) {
			if (file_->end()) {
				return PARSE_CHUNK_FAIL;
			}

			chunk_id = read_u32();
			chunk_size = read_u32();

			result_ = parse_chunk(chunk_id, chunk_size);
			if (result_ != WAV_OK) {
				return result_;
			}
		}

		return WAV_OK;
	}

private:
	File *file_;
	Result result_;

	static const uint32_t RIFF_CHUNK_ID		= 0x46464952;
	static const uint32_t FMT_CHUNK_ID		= 0x20746D66;
	static const uint32_t DATA_CHUNK_ID		= 0x61746164;
	static const uint32_t RIFF_FORMAT_WAVE	= 0x45564157;
	static const uint16_t WAVE_FORMAT_PCM	= 0x0001;

	uint32_t read_u32() {
		uint8_t data[4];
		file_->read(data, 4);
		return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	}

	Result parse_chunk(uint32_t chunk_id, uint32_t chunk_size) {
		switch (chunk_id)
		{
		case FMT_CHUNK_ID:
			if (file_->read(&fmt_chunk, 16) == false) {
				return SD_ERROR;
			}

			if (chunk_size > 16) {
				file_->advance(chunk_size - 16);
			}

			if (fmt_chunk.audio_format != WAVE_FORMAT_PCM) {
				return UNSUPPORTED_WAV_FORMAT;
			}

			if (fmt_chunk.bit_depth < 16) {
				return UNSUPPORTED_WAV_FORMAT;
			}

			if ((fmt_chunk.num_of_channels < 1) || (fmt_chunk.num_of_channels > 2)) {
				return UNSUPPORTED_WAV_FORMAT;
			}
			break;
		case DATA_CHUNK_ID:
			data_chunk.size = chunk_size;
			data_chunk.offset = file_->position();
			break;
		default:
			file_->advance(chunk_size);
			break;
		}
		return WAV_OK;
	}

};

#endif
