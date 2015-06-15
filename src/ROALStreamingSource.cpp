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
