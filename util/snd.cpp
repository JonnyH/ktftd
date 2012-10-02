/*
 * =====================================================================================
 *
 *       Filename:  snd.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/10/12 16:46:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include <cstdint>
#include <fstream>
#include <cassert>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "audio.hpp"

class SampleHeader
{
public:
	uint32_t offset;
	uint32_t size;
};
static_assert(sizeof(SampleHeader) == 8, "Expected SampleHeader to be 8 bytes");

void printUseage()
{
	std::cout << "Useage:\n./tftdsnd [SAMPLE.CAT]\n";
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printUseage();
		return EXIT_FAILURE;
	}
	std::ifstream inFile (argv[1]);
	std::vector<SampleHeader> headers;

	uint32_t lowestOffset = 0;

	do
	{
		SampleHeader header;
		inFile.read((char*)&header, 8);
		assert(header.offset > inFile.tellg());
		if (lowestOffset == 0 || header.offset < lowestOffset)
		{
			lowestOffset = header.offset;
		}
		headers.push_back(header);
	}
	while ((uint32_t)inFile.tellg()+4 < lowestOffset);
	std::cout << "Found " << headers.size() << " headers\n";

	for (auto header : headers)
	{
		static int fileNo = 0;
		ktftd::audio::AudioFile audioFile;
		audioFile.sampleRate = 11025;
		audioFile.sampleCount = header.size;
		audioFile.channels = 1;
		audioFile.bitsPerSample = 8;
		audioFile.isSignedPCM = true;
		audioFile.channelSamples.resize(1);
		char * samples = new char[header.size];
		inFile.seekg(header.offset, std::ios::beg);
		inFile.read(samples, header.size);
		audioFile.channelSamples[0] = samples;
		std::cout << "Writing file " << fileNo << " of " << audioFile.sampleCount << " samples\n";

		std::stringstream ss;
		ss << "out" << std::setw(3) << std::setfill('0') << fileNo << ".flac";
		
		audioFile.writeFLAC(ss.str().c_str());

		delete[] samples;

		fileNo ++;

	}
}
