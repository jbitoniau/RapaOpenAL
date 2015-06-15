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
#else
	#include <AL/al.h>
#endif

namespace ROAL
{

class Context;
class Buffer;

class Source
{
public:
	enum State
	{
		INITIAL = AL_INITIAL,
		PLAYING = AL_PLAYING,
		PAUSED = AL_PAUSED,
		STOPPED = AL_STOPPED
	};
	virtual ~Source();

	bool		IsValid() const { return mIsValid; }
	
	Context*	GetParentContext() const { return mParentContext; }
	
	bool		Play();
	bool		Pause();
	bool		Stop();
	bool		Rewind();

	State		GetState() const;
	
	ALfloat		GetPitch() const;
	Buffer*		GetCurrentBuffer() const;
	ALint		GetNumBuffersQueued() const;
	ALint		GetNumBuffersProcessed() const;
	
	bool		SetPitch( ALfloat _Pitch );

protected:
	friend class Context;
	Source( Context* _ParentContext );
	ALuint		GetRawSource() const { return mRawSource; }
	
	void		MakeContextCurrent() const;

	ALfloat		InternalGetFloatValue( ALenum _ParameterName ) const;
	bool		InternalSetFloatValue( ALenum _ParameterName, ALfloat _Value ) const;
	ALint		InternalGetIntValue( ALenum _ParameterName ) const;
	bool		InternalSetIntValue( ALenum _ParameterName, ALint _Value ) const;

private:
	bool		mIsValid;
	ALuint		mRawSource;
	Context*	mParentContext;
};

}
