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