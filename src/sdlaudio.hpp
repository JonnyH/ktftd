/*
 * =====================================================================================
 *
 *       Filename:  sdlaudio.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/10/12 08:14:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __SDLAUDIO_HPP
#define __SDLAUDIO_HPP

#include "audio.hpp"

#include <cstdint>
#include <vector>
#include <SDL.h>

namespace ktftd
{
namespace audio
{
namespace SDL
{

class SDLAudioDriver : public AudioDriver
{
public:
	SDLAudioDriver();
	virtual ~SDLAudioDriver();
	virtual void PlaySample(AudioFile &audioFile);
	std::vector<int16_t> sampleBuffer;
	SDL_AudioSpec obtainedAudioSpec;
};

}//namespace SDL
}//namespace audio
}//namespace ktftd

#endif //__SDLAUDIO_HPP
