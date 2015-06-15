/*
   The MIT License (MIT) (http://opensource.org/licenses/MIT)
   
   Copyright (c) 2015 Jacques Menuet
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#pragma once

#ifdef _WIN32
	#include "al.h"
	#include "alc.h"
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

namespace ROAL
{

class PlaybackDevice;
    
class Buffer
{
public:
	virtual ~Buffer();

	bool			IsValid() const { return mIsValid; }

	PlaybackDevice*	GetParentPlaybackDevice() const { return mParentPlaybackDevice; }

	enum Format
	{
		MONO8		= AL_FORMAT_MONO8,
		MONO16		= AL_FORMAT_MONO16,
		STEREO8		= AL_FORMAT_STEREO8,
		STEREO16	= AL_FORMAT_STEREO16
	};

	bool			SetBufferData( Format _Format, const ALvoid* _Data, ALsizei _DataSizeInBytes, ALsizei _Frequency );
	
	ALint			GetFrequency() const;
	ALint			GetSizeInBytes() const;
	ALint			GetNumBitsPerFrame() const;
	ALint			GetNumChannels() const;

protected:
	friend class PlaybackDevice;					
	Buffer( PlaybackDevice* _ParentPlaybackDevice );
	
	friend class StreamingSource;
	friend class StaticSource;
	ALuint			GetRawBuffer() const { return mRawBuffer; }

	void			MakeContextCurrent() const;
	
	ALint			InternalGetIntValue( ALenum _ParameterName ) const;
	bool			InternalSetIntValue( ALenum _ParameterName, ALint _Value ) const;

private:
	bool			mIsValid;
	ALuint			mRawBuffer;
	PlaybackDevice*	mParentPlaybackDevice;
};

}