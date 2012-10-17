/*
 * =====================================================================================
 *
 *       Filename:  flc.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/10/12 16:07:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "flc.hpp"

#include <map>
#include <iostream>
#include <iomanip>
#include <cassert>

namespace ktftd
{
namespace vid
{

enum FLCChunkType
{
	CT_NULL,
	CT_CEL_DATA,
	CT_COLOR_256,
	CT_DELTA_FLC,
	CT_COLOR_64,
	CT_DELTA_FLI,
	CT_BLACK,
	CT_BYTE_RUN,
	CT_FLI_COPY,
	CT_PSTAMP,
	CT_DTA_BRUN,
	CT_DTA_COPY,
	CT_DTA_LC,
	CT_LABEL,
	CT_BMP_MASK,
	CT_MLEV_MASK,
	CT_SEGMENT,
	CT_KEY_IMAGE,
	CT_KEY_PAL,
	CT_REGION,
	CT_WAVE,
	CT_USERSTRING,
	CT_RGN_MASK,
	CT_LABELEX,
	CT_SHIFT,
	CT_PATHMAP,
	CT_PREFIX,
	CT_SCRIPT,
	CT_FRAME,
	CT_SEGMENT_TABLE,
	CT_HUFFMAN_TABLE,
	CT_TFTD_AUDIO,
};

std::map<uint32_t, FLCChunkType> ChunkIDMap
{
	{3,CT_CEL_DATA},
	{4,CT_COLOR_256},
	{7,CT_DELTA_FLC},
	{11,CT_COLOR_64},
	{12,CT_DELTA_FLI},
	{13,CT_BLACK},
	{15,CT_BYTE_RUN},
	{16,CT_FLI_COPY},
	{18,CT_PSTAMP},
	{25,CT_DTA_BRUN},
	{26,CT_DTA_COPY},
	{27,CT_DTA_LC},
	{31,CT_LABEL},
	{32,CT_BMP_MASK},
	{33,CT_MLEV_MASK},
	{34,CT_SEGMENT},
	{35,CT_KEY_IMAGE},
	{36,CT_KEY_PAL},
	{37,CT_REGION},
	{38,CT_WAVE},
	{39,CT_USERSTRING},
	{40,CT_RGN_MASK},
	{41,CT_LABELEX},
	{42,CT_SHIFT},
	{43,CT_PATHMAP},
	{0xF100,CT_PREFIX},
	{0xF1E1,CT_SCRIPT},
	{0xF1FA,CT_FRAME},
	{0xF1FB,CT_SEGMENT_TABLE},
	{0xF1FC,CT_HUFFMAN_TABLE},
	{0xAAAA,CT_TFTD_AUDIO},
};

std::map<FLCChunkType, std::string> ChunkNameMap
{
	{CT_NULL,         "CT_NULL"},
	{CT_CEL_DATA,     "CT_CEL_DATA"},
	{CT_COLOR_256,    "CT_COLOR_256"},
	{CT_DELTA_FLC,    "CT_DELTA_FLC"},
	{CT_COLOR_64,     "CT_COLOR_64"},
	{CT_DELTA_FLI,    "CT_DELTA_FLI"},
	{CT_BLACK,        "CT_BLACK"},
	{CT_BYTE_RUN,     "CT_BYTE_RUN"},
	{CT_FLI_COPY,     "CT_FLI_COPY"},
	{CT_PSTAMP,       "CT_PSTAMP"},
	{CT_DTA_BRUN,     "CT_DTA_BRUN"},
	{CT_DTA_COPY,     "CT_DTA_COPY"},
	{CT_DTA_LC,       "CT_DTA_LC"},
	{CT_LABEL,        "CT_LABEL"},
	{CT_BMP_MASK,     "CT_BMP_MASK"},
	{CT_MLEV_MASK,    "CT_MLEV_MASK"},
	{CT_SEGMENT,      "CT_SEGMENT"},
	{CT_KEY_IMAGE,    "CT_KEY_IMAGE"},
	{CT_KEY_PAL,      "CT_KEY_PAL"},
	{CT_REGION,       "CT_REGION"},
	{CT_WAVE,         "CT_WAVE"},
	{CT_USERSTRING,   "CT_USERSTRING"},
	{CT_RGN_MASK,     "CT_RGN_MASK"},
	{CT_LABELEX,      "CT_LABELEX"},
	{CT_SHIFT,        "CT_SHIFT"},
	{CT_PATHMAP,      "CT_PATHMAP"},
	{CT_PREFIX,       "CT_PREFIX"},
	{CT_SCRIPT,       "CT_SCRIPT"},
	{CT_FRAME,        "CT_FRAME"},
	{CT_SEGMENT_TABLE,"CT_SEGMENT_TABLE"},
	{CT_HUFFMAN_TABLE,"CT_HUFFMAN_TABLE"},
	{CT_TFTD_AUDIO,   "CT_TFTD_AUDIO"},
};

class FLCFileHeader
{
public:
	uint32_t size;
	uint16_t type;
	uint16_t frames;
	uint16_t width;
	uint16_t height;
	uint16_t depth;
	uint16_t flags;
	uint32_t speed;
	uint16_t reserved1;
	uint32_t created;
	uint32_t creator;
	uint32_t updated;
	uint32_t updater;
	uint16_t aspect_dx;
	uint16_t aspect_dy;
	uint16_t ext_flags;
	uint16_t keyframes;
	uint16_t totalframes;
	uint32_t req_memory;
	uint16_t max_regions;
	uint16_t transp_num;
	uint8_t reserved2[24];
	uint32_t frame1Offset;
	uint32_t frame2Offset;
	uint8_t reserved3[40];
}__attribute__((packed));
static_assert(sizeof(FLCFileHeader) == 128, "Expected FLCFileHeader to be 128 bytes");

class FLCChunkHeader
{
public:
	uint32_t size;
	uint16_t type;
}__attribute__((packed));
static_assert(sizeof(FLCChunkHeader) == 6, "Expected FLCChunkHeader to be 6 bytes");

class FLCFrameChunkHeader
{
public:
	uint16_t chunks;
	uint16_t delay;
	uint16_t reserved;
	uint16_t widthOverride;
	uint16_t heightOverride;
};
static_assert(sizeof(FLCFrameChunkHeader) == 10, "Expected FLCFrameChunkHeader to be 10 bytes");

class FLCTFTDAudioChunkHeader
{
public:
	uint16_t unknown1;
	uint16_t sampleRate;
	uint16_t unknown2;
	uint16_t unknown3;
	uint16_t unknown4;
};
static_assert(sizeof(FLCTFTDAudioChunkHeader) == 10, "Expected FLCTFTDAudioChunkHeader to be 10 bytes");

class FLCFrame : public Frame
{
public:
	virtual ktftd::audio::AudioFile &getAudioFile(){return this->audio;};
	virtual ktftd::img::Image &getImage(){return this->image;};
	virtual int getFrameTime(){return this->frameTime;};
	virtual ~FLCFrame();

	ktftd::img::Image image;
	ktftd::img::PaletteImage palImage;
	ktftd::img::Palette palette;
	ktftd::audio::AudioFile audio;
	int frameTime;
};

class FLCVideo : public ktftd::vid::Video
{
public:
	FLCVideo(std::istream &inStream);
	virtual bool hasNextFrame();
	virtual int getFrameNo();
	virtual int getTotalFrameCount();
	virtual ktftd::vid::Frame &getNextFrame();
	virtual int getFrameHeight();
	virtual int getFrameWidth();
	virtual ~FLCVideo();
private:
	std::istream &inStream;
	int frameHeight;
	int frameWidth;
	int frameCount;
	int frameNumber;
	FLCFileHeader fileHeader;

	FLCFrame currentFrame;
	
};

Video* loadFLC(std::istream &inStream)
{
	return new FLCVideo(inStream);
}

class FLCChunk
{
public:
	FLCChunkHeader header;
	std::shared_ptr<char> chunkData;
};

static FLCChunk readChunk(std::istream &inStream)
{
	FLCChunk chunk;
	inStream.read((char*)&chunk.header, sizeof(chunk.header));

	auto size = chunk.header.size;
	if (chunk.header.type == 0xaaaa)
	{
		size += sizeof(FLCTFTDAudioChunkHeader);
	}
	else
	{
		size -= sizeof(chunk.header);
	}
	chunk.chunkData = std::shared_ptr<char>(new char[size]);
	inStream.read(chunk.chunkData.get(), size);
	return chunk;
}

FLCVideo::FLCVideo(std::istream &inStream)
	: inStream(inStream)
{
	inStream.read((char*)&this->fileHeader, sizeof(this->fileHeader));

	this->frameCount = this->fileHeader.frames;
	this->frameWidth = this->fileHeader.width;
	this->frameHeight = this->fileHeader.height;
	this->currentFrame.image = ktftd::img::Image(this->frameWidth, this->frameHeight);


}

int
FLCVideo::getFrameNo()
{
	return this->frameNumber;
}

int
FLCVideo::getTotalFrameCount()
{
	return this->frameCount;
}

int
FLCVideo::getFrameHeight()
{
	return this->frameHeight;
}

int FLCVideo::getFrameWidth()
{
	return this->frameWidth;
}

bool FLCVideo::hasNextFrame()
{
	return (this->frameNumber < this->frameCount);
}

Frame& FLCVideo::getNextFrame()
{
	bool gotFrame = false;
	while (!gotFrame && inStream.good() && !inStream.eof())
	{
		FLCChunk chunk = readChunk(inStream);
		assert(ChunkIDMap.count(chunk.header.type) == 1);
		auto chunkType = ChunkIDMap[chunk.header.type];
		switch(chunkType)
		{
			case CT_FRAME:
				std::cout << "CT_FRAME\n";
				//TODO: Load frame
				gotFrame = true;
				break;
			case CT_TFTD_AUDIO:
				std::cout << "CT_TFTD_AUDIO\n";
				//TODO: Load audio
				break;
			default:
				std::cout << "Unhandled chunk: " << ChunkNameMap[chunkType] << std::endl;
		}
	}
	return currentFrame;
}

FLCVideo::~FLCVideo()
{

}


FLCFrame::~FLCFrame()
{

}

};//namespace vid
};//namespace ktftd
