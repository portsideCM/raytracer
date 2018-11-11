#pragma once
class BMPWriter
{
public: static const enum BitsPerPixel {
	_1BPP = 1, 
	_8BPP = 8, 
	_16BPP = 16,
	_24BPP = 24, //Blue green red
	_32BPP = 32
};

public: static const enum DataType {
	RGB = 0,
	BGR = 1,
	GRB = 2
};

private:
	std::fstream& _fileOut;
	uint64_t pixelArraySize, bytesPrinted;
	uint32_t height, width;
	uint32_t rowWidth;
	uint8_t paddingBytes;
	const BitsPerPixel BPP;

	template<std::size_t N>
	void writeConstantSizeArray(const uint8_t(&arr)[N]);

	void putSizesInHeader(uint8_t arr[4], const uint32_t data);
public:
	BMPWriter(BMPWriter const&) = delete;
	BMPWriter(const uint16_t h, const uint16_t w, std::fstream& out, const BitsPerPixel& bppValue);
	~BMPWriter() = default;

	void writeImage(uint8_t* imageBuffer, const size_t& inputSize, const DataType& dt);
};

