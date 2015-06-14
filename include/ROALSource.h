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
