/*
 * =====================================================================================
 *
 *       Filename:  flc.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 17:08:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "flc.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <sstream>

#define FLC_OPCODE_MASK (0xC000)
#define FLC_OPCODE_PACKET_COUNT (0x0000)
#define FLC_OPCODE_LAST_PIXEL (0x8000)
#define FLC_OPCODE_LINE_SKIP (0xC000)


static char* indexedFrameBuffer;
static ktftd::img::RGBColor palette[256];
static int frameCount = 0;
int vidHeight, vidWidth;

static void ApplyFrame(FLCDeltaChunk &frameDelta)
{
	size_t pos = 0;
	for (auto &deltaPacket : frameDelta.deltaPackets)
	{
		pos += deltaPacket.pixelSkip;
		memcpy((char*)&indexedFrameBuffer[pos], deltaPacket.pixelBytes, deltaPacket.pixelCount);
		pos += deltaPacket.pixelCount;
	}
	ktftd::img::PaletteImage frameImg(vidWidth, vidHeight);
	memcpy((char*)frameImg.data.get(), indexedFrameBuffer, vidWidth*vidHeight);
	ktftd::img::Palette framePalette;
	memcpy((char*)framePalette.palette, palette, sizeof(palette));

	std::stringstream ss;
	ss << "out" << frameCount << ".png";

	frameImg.getImage(framePalette).writePNG(ss.str().c_str());
	frameCount++;

	
}

static FLCDeltaChunk* DecodeFLCDeltaFrame(size_t size, int width, int height, std::istream &inStream)
{
	FLCDeltaChunk *delta = new FLCDeltaChunk(width, height);
	uint16_t encodedLines;
	inStream.read((char*)&encodedLines, 2);

	int prevLinePixelsRemaining = 0;

	for (int line = 0; line < encodedLines; line++)
	{
		uint16_t opcode;
		bool hasLastPixel = false;
		uint8_t lastPixelValue = 0;
		uint16_t lineSkip = 0;
		
		int prevLineSkipPixels = 0;
		assert(prevLinePixelsRemaining >= 0);
		if (prevLinePixelsRemaining)
		{
			prevLineSkipPixels = prevLinePixelsRemaining;
		}
		prevLinePixelsRemaining = width;

		inStream.read((char*)&opcode, 2);
		while (opcode & FLC_OPCODE_MASK)
		{
			switch (opcode & FLC_OPCODE_MASK)
			{
				case FLC_OPCODE_LAST_PIXEL:
					hasLastPixel = true;
					lastPixelValue = (opcode & 0xFF);
					break;
				case FLC_OPCODE_LINE_SKIP:
					int16_t *opcodeSigned = (int16_t*)&opcode;
					if (*opcodeSigned < 0)
						lineSkip = -*opcodeSigned;
					else
						lineSkip = *opcodeSigned;
					break;
					
			}
			inStream.read((char*)&opcode, 2);
		}
		uint16_t packetCount = opcode;


		if (packetCount == 0)
		{
			//If no packets, only the last pixel has changed
			assert(hasLastPixel);
			DeltaPacket packet;
			packet.pixelSkip = width - 1 + (width * lineSkip) + prevLineSkipPixels;
			lineSkip = 0;
			packet.pixelCount = 1;
			packet.pixelBytes = new char[1];
			packet.pixelBytes[0] = lastPixelValue;
			delta->deltaPackets.push_back(packet);
		}
		for (int p = 0; p < packetCount; p++)
		{
			DeltaPacket packet;
			uint8_t pixelSkip;
			inStream.read((char*)&pixelSkip, 1);

			prevLinePixelsRemaining -= pixelSkip;
			assert(prevLinePixelsRemaining >= 0);


			packet.pixelSkip = ((int)pixelSkip) + (width * lineSkip) + prevLineSkipPixels;
			lineSkip = 0;

			int8_t count;
			inStream.read((char*)&count, 1);
			//TODO: Check this case occurs in FLC
			while (count == 0)
			{
				inStream.read((char*)&pixelSkip, 1);
				inStream.read((char*)&count, 1);
				packet.pixelSkip += pixelSkip;
			}


			if (count > 0)
			{
				packet.pixelCount = count*2;
				packet.pixelBytes = new char[count*2];
				inStream.read((char*)packet.pixelBytes, count*2);
				prevLinePixelsRemaining -= packet.pixelCount;
				assert(prevLinePixelsRemaining >= 0);
			}
			else if (count < 0)
			{
				int copyBytes = -count*2;
				packet.pixelCount = copyBytes;
				packet.pixelBytes = new char[copyBytes];
				prevLinePixelsRemaining -= packet.pixelCount;
				assert(prevLinePixelsRemaining >= 0);
				uint16_t copiedWord;
				inStream.read((char*)&copiedWord, 2);
				uint16_t *pixelWords = (uint16_t*)packet.pixelBytes;
				for (int i = 0; i < copyBytes/2; i++)
				{
					pixelWords[i] = copiedWord;
				}

			}
			else
			{
				assert(0);
			}

			delta->deltaPackets.push_back(packet);
		}
		
	}

	return delta;
}


static uint8_t* DecodeRLEFrame(int width, int height, std::istream &inStream)
{
	uint8_t* outputBytes = new uint8_t[width*height];
	int x = 0;
	//auto streamOffset = inStream.tellg();

	for (int y = 0; y < height; y++)
	{
		//std::cout << "y = " << y << std::endl;
		char *outLine = new char[width];
		//First byte is skipped;
		inStream.seekg(1, std::ios::cur);
		while (x < width)
		{
			//std::cout << "x = " << x << std::endl;
			char count;
			inStream.read(&count, 1);
			if (count < 0)
			{
				int copyBytes = -count;
				//std::cout << "Copying " << copyBytes << " bytes\n";
				inStream.read((char*)outLine+x, copyBytes);
				x += copyBytes;
			}
			else
			{
				int replicateCount = count;
				char replicateData;
				inStream.read(&replicateData, 1);
				//std::cout << "Replicating " << replicateCount << " bytes\n";
				for (int i = 0; i < replicateCount; i++)
				{
					outLine[x] = replicateData;
					x++;
				}
			}
		}
		memcpy(outputBytes+(width*y), outLine, width);
		x = 0;
	}

	//std::cout << "Read " << (inStream.tellg()-streamOffset) << " bytes\n";

	return outputBytes;
}

FLCChunk *loadChunk(std::istream &inStream, int imageWidth, int imageHeight)
{
	FLCChunkHeader header;
	FLCChunk *chunk;
	FLCChunkType chunkType;


	inStream.read((char*)&header, sizeof(header));
	if (inStream.bad() || inStream.eof())
	{
		//std::cout << "End of file\n";
		return new FLCChunk();
	}
	//std::cout << "Loading chunk 0x" << std::hex << header.type << std::dec << " size " << header.size << "bytes\n";
	assert(ChunkIDMap.count(header.type) == 1);
	chunkType = ChunkIDMap[header.type];
	//std::cout << "Found chunk of type " << ChunkNameMap[chunkType] << " of size " << header.size << std::endl;

	switch (chunkType)
	{
		case CT_TFTD_AUDIO:
			chunk = new FLCTFTDAudioChunk(header.size, inStream);
			break;
		case CT_FRAME:
			chunk = new FLCFrameTypeChunk(header.size, inStream, imageWidth, imageHeight);
			break;
		case CT_COLOR_256:
		{
			FLCChunkColor256 *paletteChunk = new FLCChunkColor256(header.size, inStream);
			memcpy((char*)palette, paletteChunk->palette, sizeof(paletteChunk->palette));
			chunk = paletteChunk;
			break;
		}
		case CT_BYTE_RUN:
		{
			FLCDeltaChunk *frameChunk = new FLCDeltaChunk(imageWidth, imageHeight);
			DeltaPacket packet;
			packet.pixelSkip = 0;
			packet.pixelCount = imageWidth*imageHeight;
			packet.pixelBytes = (char*)DecodeRLEFrame(imageWidth, imageHeight, inStream);
			frameChunk->deltaPackets.push_back(packet);
			ApplyFrame(*frameChunk);
			chunk = frameChunk;
			break;
		}
		//case CT_DELTA_FLI:
		//	chunk = DecodeFLIDeltaFrame(header.size, imageWidth, imageHeight, inStream);
		//	break;
		case CT_DELTA_FLC:
		{
			auto* deltaFrame = DecodeFLCDeltaFrame(header.size, imageWidth, imageHeight, inStream);
			ApplyFrame(*deltaFrame);
			chunk = deltaFrame;
			break;
		}
		default:
			std::cout << "TODO: Implement chunk type " << ChunkNameMap[chunkType] << std::endl;
			chunk = new FLCChunk(chunkType, header.size);
			size_t skipBytes = header.size - sizeof(header);
			if (skipBytes%2)
				skipBytes++;
			inStream.seekg(skipBytes, std::ios::cur);
	}
	return chunk;
}

FLCTFTDAudioChunk::FLCTFTDAudioChunk(size_t size, std::istream &inStream)
	: FLCChunk(CT_TFTD_AUDIO, size)
{
	//TFTD audio chunks seems to have a different interpretation of the size - it seems to
	//not include the 6byte flc header, or the 10byte extra audio header, and is therefore
	//only the size of the samples
	inStream.read((char*)&this->audioHeader, sizeof(this->audioHeader));
	this->sampleValues = std::unique_ptr<char[]>(new char[size]);
	inStream.read(this->sampleValues.get(), size);
}

FLCFrameTypeChunk::FLCFrameTypeChunk(size_t size, std::istream &inStream, int videoWidth, int videoHeight)
	:	FLCChunk(CT_FRAME, size)
{
	//Frame chunks contain subchunks after an extra header
	inStream.read((char*)&this->frameHeader, sizeof(this->frameHeader));
	//std::cout << "Frame header:" <<
	//	"\n\tChunks\t" << this->frameHeader.chunks << 
	//	"\n\tDelay\t" << this->frameHeader.delay <<
	//	"\n\tWidthOV\t" << this->frameHeader.widthOverride <<
	//	"\n\tHeightOV\t" << this->frameHeader.heightOverride << std::endl;
	
	this->subChunks.resize(3);

	int frameWidth = (this->frameHeader.widthOverride) ? this->frameHeader.widthOverride : videoWidth;
	int frameHeight = (this->frameHeader.widthOverride) ? this->frameHeader.widthOverride : videoHeight;

	for (int chunk = 0; chunk < this->frameHeader.chunks; chunk++)
	{
		this->subChunks[chunk] = loadChunk(inStream, frameWidth, frameHeight);
	}
}

FLCChunkColor256::FLCChunkColor256(size_t size, std::istream &inStream)
	: FLCChunk(CT_COLOR_256, size)
{
	inStream.read((char*)&this->packetCount, 2);
	//std::cout << "CT_COLOR_256 section w/ " << this->packetCount << " packets\n";
	

	for (int packet = 0; packet < this->packetCount; packet++)
	{
		uint8_t skipByte;
		uint8_t copyByte;
		inStream.read((char*)&skipByte, 1);
		inStream.read((char*)&copyByte, 1);

		int copy = copyByte;
		int skip = skipByte;
		//std::cout << "Packet " << packet << " skip " << (int)skip << " copy " << (int)copy << std::endl;

		//If the copy count is zero, there are 256 rgb triplets (complete palette replacement)
		if (copy == 0)
		{
			copy = 256;
			skip = 0;
		}

		inStream.read((char*)&this->palette[skip], 3*copy);
	}
}

int main(int argc, char **argv)
{
	assert(argc == 2);
	std::ifstream inFile(argv[1]);

	FLCFile flc;

	inFile.read((char*)&flc.header, sizeof(flc.header));

	//std::cout << "FLC header:\n" <<
	//"\tsize\t=\t" << flc.header.size << std::endl <<
	//"\ttype\t=\t" << flc.header.type << std::endl <<
	//"\tframes\t=\t" << flc.header.frames << std::endl <<
	//"\twidth\t=\t" << flc.header.width << std::endl <<
	//"\theight\t=\t" << flc.header.height << std::endl <<
	//"\tdepth\t=\t" << flc.header.depth << std::endl <<
	//"\tflags\t=\t" << flc.header.flags << std::endl <<
	//"\tspeed\t=\t" << flc.header.speed << std::endl <<
	//"\taspect_dx\t=\t" << flc.header.aspect_dx << std::endl <<
	//"\taspect_dy\t=\t" << flc.header.aspect_dy << std::endl <<
	//"\text_flags\t=\t" << flc.header.ext_flags << std::endl <<
	//"\tkeyframes\t=\t" << flc.header.keyframes << std::endl <<
	//"\ttotalframes\t=\t" << flc.header.totalframes << std::endl <<
	//"\treq_memory\t=\t" << flc.header.req_memory << std::endl <<
	//"\tmax_regions\t=\t" << flc.header.max_regions << std::endl <<
	//"\ttransp_num\t=\t" << flc.header.transp_num << std::endl <<
	//"\tframe1offset\t=\t" << flc.header.frame1Offset << std::endl <<
	//"\tframe2offset\t=\t" << flc.header.frame2Offset << std::endl;

	vidWidth = flc.header.width;
	vidHeight = flc.header.height;

	indexedFrameBuffer = new char[flc.header.width*flc.header.height];

	while (inFile.good() || !inFile.eof())
	{
		loadChunk(inFile, flc.header.width, flc.header.height);
	}


}
