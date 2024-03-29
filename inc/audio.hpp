/*
 * =====================================================================================
 *
 *       Filename:  audio.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/09/12 09:40:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __AUDIO_HPP
#define __AUDIO_HPP

#include <cstdint>
#include <vector>

namespace ktftd
{
namespace audio
{

class AudioFile
{
public:
	int sampleRate;
	uint32_t sampleCount;
	uint32_t channels;
	uint32_t bitsPerSample;
	std::vector<char*> channelSamples;
	bool isSignedPCM;
	void writeFLAC(const char* fileName);
};

class AudioDriver
{
public:

	virtual void PlaySample(AudioFile &audioFile) = 0;

	static AudioDriver& getAudioDriver();
	static void setAudioDriver(AudioDriver *drv);
};

}//namespace audio
}//namespace ktftd

#endif //__AUDIO_HPP
