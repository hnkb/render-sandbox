
#include "Compression.h"
#include <brotli/decode.h>

void decompress(
	const uint8_t* inputBuffer, const size_t inputSize,
	std::function<uint8_t* ()> outputBuffer, std::function<size_t()> outputSize, std::function<void(size_t)> outputResize)
{
		outputResize(inputSize * 20);
		size_t actualSize = outputSize();
		if (!BrotliDecoderDecompress(inputSize, inputBuffer, &actualSize, outputBuffer()))
			throw std::runtime_error(" failed");
		outputResize(actualSize);
}
