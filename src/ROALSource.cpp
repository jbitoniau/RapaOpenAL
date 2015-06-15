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
#include "ROALSource.h"

#include <assert.h>
#include "ROALContext.h"
#include "ROALBuffer.h"
#include "ROALPlaybackDevice.h"

namespace ROAL
{

Source::Source( Context* _ParentContext )
	:	mIsValid(false),
		mRawSource(0),
		mParentContext( _ParentContext )
{
	MakeContextCurrent();
	alGetError();
	alGenSources( 1, &mRawSource );			// TO CHECK!!! the current context needs to be set?
	assert( mRawSource );
	ALenum error = alGetError();				//	To return as error if needed
	assert( error==AL_NO_ERROR );
	mIsValid = (error==AL_NO_ERROR);
}

Source::~Source()
{	
	// If the object failed to construct, there's nothing to do in the destructor
	if ( !IsValid() )
		return;

	// Delete the Raw Source
	MakeContextCurrent();
	alGetError();
	alDeleteSources( 1, &mRawSource );
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	mRawSource = NULL;

	// Remove this Source from its parent Context
	assert( GetParentContext() );
	GetParentContext()->RemoveSource( this );
}

bool Source::Play()
{
	MakeContextCurrent();
	alGetError();
	alSourcePlay( GetRawSource() );
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return (error==AL_NO_ERROR);
}

bool Source::Pause()
{
	MakeContextCurrent();
	alGetError();
	alSourcePause( GetRawSource() );
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return (error==AL_NO_ERROR);
}

bool Source::Stop()
{
	MakeContextCurrent();
	alGetError();
	alSourceStop( GetRawSource() );
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return (error==AL_NO_ERROR);
}

bool Source::Rewind()
{
	MakeContextCurrent();
	alGetError();
	alSourceRewind( GetRawSource() );
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return (error==AL_NO_ERROR);
}

Source::State Source::GetState() const
{
	MakeContextCurrent();
	ALint l_State = InternalGetIntValue( AL_SOURCE_STATE );
	return static_cast<State>(l_State);
}

ALfloat Source::GetPitch() const
{
	MakeContextCurrent();
	return InternalGetFloatValue( AL_PITCH );
}

Buffer* Source::GetCurrentBuffer() const
{
	MakeContextCurrent();
	Buffer* l_Buffer = NULL;
	ALuint l_RawBuffer = InternalGetIntValue( AL_BUFFERS_QUEUED );
	if ( l_RawBuffer!=0 )
	{
		l_Buffer = GetParentContext()->GetParentPlaybackDevice()->GetBufferFromRawBuffer( l_RawBuffer );
	}
	return l_Buffer;
}

ALint Source::GetNumBuffersQueued() const
{
	MakeContextCurrent();
	return InternalGetIntValue( AL_BUFFERS_QUEUED );
}

ALint Source::GetNumBuffersProcessed() const
{
	MakeContextCurrent();
	return InternalGetIntValue( AL_BUFFERS_PROCESSED );
}

bool Source::SetPitch( ALfloat _Pitch )
{
	MakeContextCurrent();
	return InternalSetFloatValue( AL_PITCH, _Pitch );
}

void Source::MakeContextCurrent() const
{
	GetParentContext()->MakeContextCurrent();
}

ALfloat Source::InternalGetFloatValue( ALenum _ParameterName ) const
{
	ALfloat l_Value = 0.f;
	alGetError(); 
	alGetSourcef( GetRawSource(), _ParameterName, &l_Value ); 
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return l_Value;
}

bool Source::InternalSetFloatValue( ALenum _ParameterName, ALfloat _Value ) const
{
	alGetError(); 
	alSourcef( GetRawSource(), _ParameterName, _Value ); 
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return ( error==AL_NO_ERROR );
}

ALint Source::InternalGetIntValue( ALenum _ParameterName ) const
{
	ALint l_Value = 0;
	alGetError(); 
	alGetSourcei( GetRawSource(), _ParameterName, &l_Value ); 
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return l_Value;
}

bool Source::InternalSetIntValue( ALenum _ParameterName, ALint _Value ) const
{
	alGetError(); 
	alSourcei( GetRawSource(), _ParameterName, _Value ); 
	ALenum error = alGetError();
assert( error==AL_NO_ERROR );
	return ( error==AL_NO_ERROR );
}

}