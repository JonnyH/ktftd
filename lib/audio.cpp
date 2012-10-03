/*
 * =====================================================================================
 *
 *       Filename:  audio.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/09/12 09:44:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "audio.hpp"
#include <FLAC++/encoder.h>
#include <cassert>

namespace ktftd
{
namespace audio
{

void
AudioFile::writeFLAC(const char* fileName)
{
	FLAC::Encoder::File flacFile;
	flacFile.set_verify(true);
	flacFile.set_compression_level(5);
	flacFile.set_channels(this->channels);
	flacFile.set_bits_per_sample(this->bitsPerSample);
	flacFile.set_sample_rate(this->sampleRate);
	flacFile.set_total_samples_estimate(this->sampleCount);
	auto init_status = flacFile.init(fileName);
	if (init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
	{
		assert(0);
	}
	bool ok = true;
	int32_t *interleavedBuffer = new int32_t[this->sampleCount*this->channels];
	int32_t *bufferPos = interleavedBuffer;
	for (size_t s = 0; s < this->sampleCount; s++)
	{
		for (unsigned int c = 0; c < this->channels; c++)
		{
			switch (this->bitsPerSample)
			{
				case 8:
				{
					int32_t sampleValue = this->isSignedPCM ? ((int8_t*)this->channelSamples[c])[s] : ((uint8_t*)this->channelSamples[c])[s]-128;
					*bufferPos = sampleValue;
					bufferPos++;
					break;
				}
				default:
					assert(0);
			}
		}
	}
	ok = flacFile.process_interleaved(interleavedBuffer, this->sampleCount);
	assert(ok);
	ok = flacFile.finish();
	assert(ok);
	delete[] interleavedBuffer;
}

static AudioDriver *g_driver = NULL;

AudioDriver&
AudioDriver::getAudioDriver()
{
	return *g_driver;
}

void
AudioDriver::setAudioDriver(AudioDriver *drv)
{
	g_driver = drv;
}

}//namespace audio
}//namespace ktftd
