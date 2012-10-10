/*
 * =====================================================================================
 *
 *       Filename:  vid.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/10/12 16:11:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vid.hpp"
#include "flc.hpp"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <sstream>

void printUsage()
{
	std::cout << "Usage:\n"
	          << "./ktftdvid [video.vid]\n";
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printUsage();
		return EXIT_FAILURE;
	}
	std::ifstream videoFile(argv[1]);
	auto* video = ktftd::vid::loadFLC(videoFile);

	for (int frameNo = 0; frameNo < video->getTotalFrameCount(); frameNo++)
	{
		std::stringstream audioOutName;
		audioOutName << "out" << std::setw(6) << std::setfill('0') << frameNo << ".flac";
		std::stringstream imageOutName;
		imageOutName << "out" << std::setw(6) << std::setfill('0') << frameNo << ".png";
		assert(video->hasNextFrame());
		auto &frame = video->getNextFrame();
		frame.getImage().writePNG(imageOutName.str().c_str());
		frame.getAudioFile().writeFLAC(audioOutName.str().c_str());
	}

	delete video;
}
