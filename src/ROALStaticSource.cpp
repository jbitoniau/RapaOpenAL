#include "ROALStaticSource.h"

#include <assert.h>
#include "ROALBuffer.h"
#include "ROALPlaybackDevice.h"

namespace ROAL
{

StaticSource::StaticSource( Context* _ParentContext )
	: Source( _ParentContext )	
{
}

bool StaticSource::AttachBuffer( Buffer* _Buffer )
{
	assert( _Buffer );
	if ( !_Buffer )
		return false;

	MakeContextCurrent();
	alGetError();
	ALuint l_RawBuffer = _Buffer->GetRawBuffer();
	alSourcei( GetRawSource(), AL_BUFFER, l_RawBuffer );
	ALenum error = alGetError();
	return (error==AL_NO_ERROR);
}

bool StaticSource::DetachBuffer()
{
	MakeContextCurrent();
	alGetError();
	ALuint l_NoBuffer = AL_NONE;
	alSourcei( GetRawSource(), AL_BUFFER, l_NoBuffer );
	ALenum error = alGetError();
	return (error==AL_NO_ERROR);
}

}