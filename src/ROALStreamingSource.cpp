#include "ROALStreamingSource.h"

#include <assert.h>
#include <cstring>		// For NULL macro
#include "ROALBuffer.h"
#include "ROALContext.h"
#include "ROALPlaybackDevice.h"

namespace ROAL
{

StreamingSource::StreamingSource( Context* _ParentContext )
	:	Source( _ParentContext )
{
}

bool StreamingSource::QueueBuffer( Buffer* _Buffer )
{
	assert( _Buffer );
	if ( !_Buffer )
		return false;

	MakeContextCurrent();	
	alGetError();
	ALuint l_RawBuffer = _Buffer->GetRawBuffer();
	alSourceQueueBuffers( GetRawSource(), 1, &l_RawBuffer );
	ALenum error = alGetError();
assert( error==AL_NO_ERROR );
	return (error==AL_NO_ERROR);
}

Buffer* StreamingSource::UnqueueBuffer()
{
	Buffer* l_Buffer = NULL;

	MakeContextCurrent();
	alGetError();
	ALuint l_RawBuffer = 0;
	alSourceUnqueueBuffers( GetRawSource(), 1, &l_RawBuffer );
	ALenum error = alGetError();
	if ( error==AL_NO_ERROR )
	{
		assert( l_RawBuffer!=0 );
		l_Buffer = GetParentContext()->GetParentPlaybackDevice()->GetBufferFromRawBuffer( l_RawBuffer );
	}
	return l_Buffer;
}


}
