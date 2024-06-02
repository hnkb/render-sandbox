
#pragma once

#include <vector>
#include <stdint.h>
#include <functional>

void decompress(
	const uint8_t* inputBuffer,
	const size_t inputSize,
	std::function<uint8_t*()> outputBuffer,
	std::function<size_t()> outputSize,
	std::function<void(size_t)> outputResize);

template <class OutputType = uint8_t, template <class, class...> class Container, class... Rest>
std::vector<OutputType> decompress(const Container<uint8_t, Rest...>& compressedBuffer)
{
	std::vector<OutputType> outputBuffer;
	decompress(
		compressedBuffer.data(),
		compressedBuffer.size(),
		[&]() { return (uint8_t*)outputBuffer.data(); },                        // data
		[&]() { return sizeof(OutputType) * outputBuffer.size(); },             // size
		[&](size_t size) { outputBuffer.resize(size / sizeof(OutputType)); });  // resize
	return outputBuffer;
}

template <class OutputType = uint8_t>
std::vector<OutputType> decompress(const uint8_t* inputBuffer, const size_t inputSize)
{
	std::vector<OutputType> outputBuffer;
	decompress(
		inputBuffer,
		inputSize,
		[&]() { return (uint8_t*)outputBuffer.data(); },                        // data
		[&]() { return sizeof(OutputType) * outputBuffer.size(); },             // size
		[&](size_t size) { outputBuffer.resize(size / sizeof(OutputType)); });  // resize
	return outputBuffer;
}
