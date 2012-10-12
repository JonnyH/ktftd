/*
 * =====================================================================================
 *
 *       Filename:  vic.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/10/12 12:54:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "audio.hpp"
#include "palette_image.hpp"

#ifndef __VID_HPP
#define __VID_HPP

namespace ktftd
{
namespace vid
{

class Frame
{
public:
	virtual ktftd::audio::AudioFile &getAudioFile() = 0;
	virtual ktftd::img::Image &getImage() = 0;
	virtual ~Frame() = 0;
	virtual int getFrameTime() = 0;
};

class Video
{
public:
	virtual bool hasNextFrame() = 0;
	virtual int getFrameNo() = 0;
	virtual int getTotalFrameCount() = 0;
	virtual Frame& getNextFrame() = 0;
	virtual int getFrameHeight() = 0;
	virtual int getFrameWidth() = 0;
	virtual ~Video() = 0;
};

};//namespace vid
};//namespace ktftd

#endif //__VID_HPP
