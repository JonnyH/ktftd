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


FLCChunk *loadChunk(std::istream &inStream)
{
	FLCChunkHeader header;
	FLCChunk *chunk;
	FLCChunkType chunkType;


	inStream.read((char*)&header, sizeof(header));
	if (inStream.bad() || inStream.eof())
	{
		std::cout << "End of file\n";
		return new FLCChunk();
	}
	std::cout << "Loading chunk 0x" << std::hex << header.type << std::dec << " size " << header.size << "bytes\n";
	assert(ChunkIDMap.count(header.type) == 1);
	chunkType = ChunkIDMap[header.type];
	std::cout << "Found chunk of type " << ChunkNameMap[chunkType] << " of size " << header.size << std::endl;

	switch (chunkType)
	{
		case CT_TFTD_AUDIO:
			chunk = new FLCTFTDAudioChunk(header.size, inStream);
			break;
		case CT_FRAME:
			chunk = new FLCFrameTypeChunk(header.size, inStream);
			break;
		case CT_COLOR_256:
			chunk = new FLCChunkColor256(header.size, inStream);
			break;
		default:
			std::cout << "TODO: Other chunk types?\n";
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

FLCFrameTypeChunk::FLCFrameTypeChunk(size_t size, std::istream &inStream)
	:	FLCChunk(CT_FRAME, size)
{
	//Frame chunks contain subchunks after an extra header
	inStream.read((char*)&this->frameHeader, sizeof(this->frameHeader));
	std::cout << "Frame header:" <<
		"\n\tChunks\t" << this->frameHeader.chunks << 
		"\n\tDelay\t" << this->frameHeader.delay <<
		"\n\tWidthOV\t" << this->frameHeader.widthOverride <<
		"\n\tHeightOV\t" << this->frameHeader.heightOverride << std::endl;
	
	this->subChunks.resize(3);

	for (int chunk = 0; chunk < this->frameHeader.chunks; chunk++)
	{
		this->subChunks[chunk] = loadChunk(inStream);
	}
}

FLCChunkColor256::FLCChunkColor256(size_t size, std::istream &inStream)
	: FLCChunk(CT_COLOR_256, size)
{
	inStream.read((char*)&this->packetCount, 2);
	std::cout << "CT_COLOR_256 section w/ " << this->packetCount << " packets\n";
	

	for (int packet = 0; packet < this->packetCount; packet++)
	{
		uint8_t skipByte;
		uint8_t copyByte;
		inStream.read((char*)&skipByte, 1);
		inStream.read((char*)&copyByte, 1);

		int copy = copyByte;
		int skip = skipByte;
		std::cout << "Packet " << packet << " skip " << (int)skip << " copy " << (int)copy << std::endl;

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

	std::cout << "FLC header:\n" <<
	"\tsize\t=\t" << flc.header.size << std::endl <<
	"\ttype\t=\t" << flc.header.type << std::endl <<
	"\tframes\t=\t" << flc.header.frames << std::endl <<
	"\twidth\t=\t" << flc.header.width << std::endl <<
	"\theight\t=\t" << flc.header.height << std::endl <<
	"\tdepth\t=\t" << flc.header.depth << std::endl <<
	"\tflags\t=\t" << flc.header.flags << std::endl <<
	"\tspeed\t=\t" << flc.header.speed << std::endl <<
	"\taspect_dx\t=\t" << flc.header.aspect_dx << std::endl <<
	"\taspect_dy\t=\t" << flc.header.aspect_dy << std::endl <<
	"\text_flags\t=\t" << flc.header.ext_flags << std::endl <<
	"\tkeyframes\t=\t" << flc.header.keyframes << std::endl <<
	"\ttotalframes\t=\t" << flc.header.totalframes << std::endl <<
	"\treq_memory\t=\t" << flc.header.req_memory << std::endl <<
	"\tmax_regions\t=\t" << flc.header.max_regions << std::endl <<
	"\ttransp_num\t=\t" << flc.header.transp_num << std::endl <<
	"\tframe1offset\t=\t" << flc.header.frame1Offset << std::endl <<
	"\tframe2offset\t=\t" << flc.header.frame2Offset << std::endl;

	while (inFile.good() || !inFile.eof())
	{
		loadChunk(inFile);
	}


}
