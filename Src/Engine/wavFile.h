#ifndef WavFile_h
#define WavFile_h

class WavFile {

public:

	struct Riff_chunk {
		uint32_t size;
		uint32_t format;
	};

	struct Fmt_chunk {
		uint16_t audio_format;
		uint16_t num_of_channels;
		uint32_t sample_rate;
		uint32_t byte_rate;
		uint16_t block_allign;
		uint16_t bit_depth;
	};

	struct Data_chunk {
		uint32_t size;
		uint32_t offset;
	};

	Riff_chunk riff;
	Fmt_chunk format;
	Data_chunk data;
};

#endif
