/*
 * =====================================================================================
 *
 *       Filename:  flc.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/09/12 17:09:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __FLC_HPP
#define __FLC_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

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

class FLCChunkHeader
{
public:
	uint32_t size;
	uint16_t type;
}__attribute__((packed));
static_assert(sizeof(FLCChunkHeader) == 6, "Expected FLCChunkHeader to be 6 bytes");

class FLCChunk
{
public:
	FLCChunk(FLCChunkType type, size_t size):type(type),size(size){}
	FLCChunk():type(CT_NULL),size(6){}
	FLCChunkType type;
	size_t size;
	virtual ~FLCChunk(){};
};

class FLCFrameTypeChunk : public FLCChunk
{
public:
	FLCFrameTypeChunk(size_t size, std::istream &inStream);
	class
	{
	public:
		uint16_t chunks;
		uint16_t delay;
		uint16_t reserved;
		uint16_t widthOverride;
		uint16_t heightOverride;
	}frameHeader;
	static_assert(sizeof(frameHeader) == 10, "Expected frameHeader to be 10 bytes");
	std::vector<FLCChunk*> subChunks;
	
};

class FLCTFTDAudioChunk : public FLCChunk
{
public:
	FLCTFTDAudioChunk(size_t size, std::istream &inStream);
	class
	{
	public:
		uint16_t unknown1;
		uint16_t sampleRate;
		uint16_t unknown2;
		uint16_t unknown3;
		uint16_t unknown4;
	}audioHeader;
	static_assert(sizeof(audioHeader) == 10, "Expected audioHeader to be 10 bytes");
	std::unique_ptr<char[]> sampleValues;
};

class SegmentTable
{
public:
	uint32_t size;
	uint16_t type;
	uint16_t segmentCount;
}__attribute__((packed));
static_assert(sizeof(SegmentTable) == 8, "Expected SegmentTable to be 4 bytes");


class Segment
{
public:
	uint32_t size;
	uint16_t type;
	uint16_t label;
	uint8_t reserved1[2];
	uint16_t cont_image;
	uint16_t last_image;
	uint16_t flags;
	uint16_t frames;
	uint32_t frame1Offset;
	uint32_t frame2Offset;
	uint16_t nextSegment;
	uint16_t repeatCount;
	uint8_t reserved2[2];
}__attribute__((packed));

static_assert(sizeof(Segment) == 32, "Expected Segment to be 32 bytes");

class FLCFile
{
public:

#define EXT_FLAGS_SEGMENTS (0x00000001)
#define EXT_FLAGS_REGULAR_KEYFRAMES (0x00000002)
	class
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
	} __attribute__((packed))header;
	static_assert(sizeof(FLCFile::header) == 128, "Expected FLC header to be 128 bytes");

	std::vector<Segment> segments;


};


#endif //__FLC_HPP
