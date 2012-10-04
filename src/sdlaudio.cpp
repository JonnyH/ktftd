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
#include <memory>
#include <cassert>
#include <samplerate.h>

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
	int sampleBufferLength = MIN(length, (int)driver->sampleBuffer.size()*2);
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

static void* convertAudio(void* srcBuffer, int srcSampleCount, int srcFreq, int srcBits, bool srcIsSigned,
                             int &dstSampleCount, int dstFreq, int dstBits, bool dstIsSigned)
{
	std::unique_ptr<float[]> srcFloatBuffer, dstFloatBuffer;
	srcFloatBuffer = std::unique_ptr<float[]>(new float[srcSampleCount]);
	void* outputBuffer = nullptr;

	switch (srcBits)
	{
		case 8:
		{
			if (srcIsSigned)
			{
				int8_t *i8SrcBuffer = static_cast<int8_t*>(srcBuffer);
				for (int i = 0; i < srcSampleCount; i++)
				{
					srcFloatBuffer[i] = (float)i8SrcBuffer[i] / 127.0f;
				}
			}
			else
			{
				assert(0);
			}
			break;
		}
		default:
			assert(0);
	}

	double resampleRatio = (double)dstFreq/(double)srcFreq;

	dstSampleCount = srcSampleCount*resampleRatio;

	dstFloatBuffer = std::unique_ptr<float[]>(new float[dstSampleCount]);

	SRC_DATA resampleSrc;
	resampleSrc.data_in = srcFloatBuffer.get();
	resampleSrc.data_out = dstFloatBuffer.get();
	resampleSrc.input_frames = srcSampleCount;
	resampleSrc.output_frames = dstSampleCount;
	resampleSrc.src_ratio = resampleRatio;

	int ret = src_simple(&resampleSrc, SRC_SINC_BEST_QUALITY, 1);

	if (ret != 0)
	{
		std::cerr << "Failed to resample data, error: " << src_strerror(ret) << std::endl;
		return nullptr;
	}

	assert(resampleSrc.output_frames_gen == dstSampleCount);
	assert(resampleSrc.input_frames_used == srcSampleCount);

	switch (dstBits)
	{
		case 16:
		{
			if (dstIsSigned)
			{
				int16_t* dstBuffer = new int16_t[dstSampleCount];
				for (int i = 0; i < dstSampleCount; i++)
				{
					dstBuffer[i] = dstFloatBuffer[i]*(1<<15);
				}
				outputBuffer = dstBuffer;
			}
			else
			{
				assert(0);
			}
			break;
		}
		default:
			assert(0);
	}

	return outputBuffer;
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
		if ((int)this->sampleBuffer.size() < sampleCount)
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
