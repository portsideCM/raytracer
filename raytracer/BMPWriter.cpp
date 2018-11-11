#include "stdafx.h"
#include "BMPWriter.h"


void BMPWriter::putSizesInHeader(uint8_t arr[4], uint32_t data)
{
	for (int i = 0; i < 4; i++)
		arr[i] = static_cast<uint8_t>(data >> (8 * i));
}

BMPWriter::BMPWriter(const uint16_t h, const uint16_t w, std::fstream& out, const BitsPerPixel& bppValue)
	: height(h), width(w), _fileOut(out), BPP(bppValue), bytesPrinted(0)
{
	if (BPP != BitsPerPixel::_24BPP)
		throw new std::invalid_argument("Sizes other than 24 bits not yet implemented");

	if (height < 0 || width < 0)
		throw new std::invalid_argument("Negative values for height or width");
	
	rowWidth = 4 * ((BPP * width + 31) / 32);
	paddingBytes = rowWidth - ((BPP * width) / 8);
	pixelArraySize = rowWidth * height;

	uint8_t BMheader[] = {
		0x42, 0x4D, //"BM"
		0x00, 0x00, 0x00, 0x00, //Blank space for the total size (Placeholder)
		0x00, 0x00, 0x00, 0x00, //Unused
		0x36, 0x00, 0x00, 0x00, //Default offset for image data is (14 + 40)
	};

	uint8_t DIBheader[] = {
		0x28, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, //Width in pixels (Placeholder)
		0x00, 0x00, 0x00, 0x00, //Height in pixels (Placeholder)
		0x01, 0x00, //Color planes
		static_cast<uint8_t>(BPP), 0x00, //Number of bits per pixel (Placeholder)
		0x00, 0x00, 0x00, 0x00, //No compression
		0x00, 0x00, 0x00, 0x00, //Size of image in bytes + padding (Placeholder)
		0x00, 0x00, 0x00, 0x00, //Supposed to be the print resolution of the image but whatever
		0x00, 0x00, 0x00, 0x00, //Ditto
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	}; 

	//Now fill the placeholders
	putSizesInHeader(&BMheader[2], 54 + pixelArraySize);
	putSizesInHeader(&DIBheader[4], width);
	putSizesInHeader(&DIBheader[8], height);
	putSizesInHeader(&DIBheader[20], pixelArraySize);

	//Print headers and we're now we can print
	writeConstantSizeArray(BMheader);
	writeConstantSizeArray(DIBheader);
}

void BMPWriter::writeImage(uint8_t* imageBuffer, const size_t& inputSize, const DataType & dt)
{
	if (inputSize > (pixelArraySize - bytesPrinted))
		throw new std::invalid_argument("Too many bytes given");
	if (BPP == BitsPerPixel::_24BPP) {
		if (inputSize % 3 != 0)
			throw new std::invalid_argument("Invalid 24-bit input given");
		//Start at the back because bitmap is weird
		size_t bytesPerRow = (_24BPP / 8) * width;
		size_t numRows = inputSize / bytesPerRow;
		for (int currRow = (numRows - 1); currRow >= 0; --currRow) {
			for (int i = (currRow * bytesPerRow); i < ((currRow + 1) * bytesPerRow); i+=3) {
				uint8_t bgr[3];
				if (dt == DataType::BGR) {
					bgr[0] = imageBuffer[i];
					bgr[1] = imageBuffer[i + 1];
					bgr[2] = imageBuffer[i + 2];
				}
				else if (dt == DataType::RGB) {
					bgr[0] = imageBuffer[i + 2];
					bgr[1] = imageBuffer[i + 1];
					bgr[2] = imageBuffer[i];
				}
				else if (dt == DataType::GRB) {
					bgr[0] = imageBuffer[i + 2];
					bgr[1] = imageBuffer[i];
					bgr[2] = imageBuffer[i + 1];
				}
				writeConstantSizeArray(bgr);
			}
			//Now write the padding
			for(int i = 0; i < paddingBytes; ++i)	
				_fileOut.put(0);
		}
		bytesPrinted += inputSize + numRows * paddingBytes;
	}
}

template<std::size_t N>
inline void BMPWriter::writeConstantSizeArray(const uint8_t(&arr)[N])
{
	_fileOut.write(reinterpret_cast<const char*>(arr), sizeof(arr));
}
