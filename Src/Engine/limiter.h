#ifndef Limiter_h
#define Limiter_h

// Pinchenettes

class Limiter {
public:

	inline void process(int16_t *data, int size, const int inc = 1) {
		while (size--) {
			float s = int_to_float(*data);
			float error = fabsf(s) - peak_;
			peak_ += (error > 0 ? 0.05f : 0.00002f) * error;
			float gain = (peak_ <= 1.0f ? 1.0f : 1.0f / peak_);
			*data = float_to_int(s * gain * 0.8f);
			data += inc;
		}
	}
private:
	float peak_ = 0.5f;

	inline float int_to_float(int value) {
		return value * (1.f * 32767.f);
	}

	inline int float_to_int(float value) {
		int value_ = value * 32767;
		return value_ > 32767 ? 32767 : (value_ < -32768 ? -32768 : value_);
	}
};


#endif
