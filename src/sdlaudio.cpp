/*
 * =====================================================================================
 *
 *       Filename:  sdlaudio.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/10/12 08:13:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "sdlaudio.hpp"
#include <SDL.h>
#include <iostream>

namespace ktftd
{
namespace audio
{
namespace SDL
{

#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif

static void
CallBack(void* userData, uint8_t *stream, int length)
{
	auto driver = static_cast<SDLAudioDriver*>(userData);
	int sampleBufferLength = MIN(length, driver->sampleBuffer.size()*2);
	if (sampleBufferLength > 0)
		memcpy(stream, &driver->sampleBuffer[0], sampleBufferLength);
	int remainingLength = length - sampleBufferLength;
	memset(stream+sampleBufferLength, 0, remainingLength);
}

SDLAudioDriver::SDLAudioDriver()
{
	SDL_AudioSpec desired;
	desired.format = AUDIO_S16SYS;
	desired.freq=44100;
	desired.channels=1;
	desired.samples=4096;
	desired.callback = CallBack;
	desired.userdata = this;

	if (SDL_OpenAudio(&desired, &this->obtainedAudioSpec) < 0)
	{
		std::cerr << "Failed to open SDL audio driver: " << SDL_GetError() << std::endl;
	}
	SDL_PauseAudio(0);

}

SDLAudioDriver::~SDLAudioDriver()
{

}

static void* convertAudio(void* sourceBuffer, int sourceSampleCount, int sourceFreq, int sourceBits, bool sourceIsSigned,
                             int &dstSampleCount, int dstFreq, int dstBits, bool dstIsSigned)
{
	return nullptr;
}

void
SDLAudioDriver::PlaySample(AudioFile &file)
{
	int16_t *sampleBuffer;
	int sampleCount;
	bool bFreeSampleBuffer = false;
	if (file.sampleRate != this->obtainedAudioSpec.freq ||
	    file.bitsPerSample != 16 ||
			file.isSignedPCM == false)
	{
		sampleBuffer = static_cast<int16_t*>(convertAudio(file.channelSamples[0], file.sampleCount, file.sampleRate, file.bitsPerSample, file.isSignedPCM, sampleCount, this->obtainedAudioSpec.freq, 16, true));
		bFreeSampleBuffer = true;
	}
	else
	{
		sampleBuffer = reinterpret_cast<int16_t*>(file.channelSamples[0]);
		sampleCount = file.sampleCount;
		bFreeSampleBuffer = false;
	}

	{
		SDL_LockAudio();
		if (this->sampleBuffer.size() < sampleCount)
		{
			this->sampleBuffer.resize(sampleCount);
		}
		SDL_MixAudio((Uint8*)&this->sampleBuffer[0], (const Uint8*)sampleBuffer, sampleCount*2, SDL_MIX_MAXVOLUME);
		SDL_UnlockAudio();
	}

	if (bFreeSampleBuffer)
	{
		delete[] sampleBuffer;
	}
}

}//namespace SDL
}//namespace audio
}//namespace ktftd
