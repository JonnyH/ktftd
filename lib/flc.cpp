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
namespace ktftd
{
namespace vid
{

class FLCFrame : public Frame
{
public:
	virtual ktftd::audio::AudioFile &getAudioFile(){return this->audio;};
	virtual ktftd::img::Image &getImage(){return this->image;};
	virtual int getFrameTime(){return this->frameTime;};
	virtual ~FLCFrame();

	ktftd::img::Image image;
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

	FLCFrame currentFrame;
	std::unique_ptr<char*>fileBytes;
	
};

Video* loadFLC(std::istream &inStream)
{
	return new FLCVideo(inStream);
}

FLCVideo::FLCVideo(std::istream &inStream)
	: inStream(inStream)
{

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

}

Frame& FLCVideo::getNextFrame()
{

}

FLCVideo::~FLCVideo()
{

}


FLCFrame::~FLCFrame()
{

}

};//namespace vid
};//namespace ktftd
