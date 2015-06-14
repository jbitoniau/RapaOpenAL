#include "ROALBuffer.h"

#include <assert.h>
#include "ROALPlaybackDevice.h"

namespace ROAL
{

Buffer::Buffer( PlaybackDevice* _ParentPlaybackDevice )
	:	mIsValid(false),
		mRawBuffer(0),
		mParentPlaybackDevice( _ParentPlaybackDevice )
{
	assert( !GetParentPlaybackDevice()->GetContexts().empty() );	// A buffer can only be created if the parent PlaybackDevice has at least a context that we can make current
	if ( GetParentPlaybackDevice()->GetContexts().empty() )
	{
		mIsValid = false;
		return;
	}
	MakeContextCurrent();
	alGetError();
	alGenBuffers( 1, &mRawBuffer );
	assert( mRawBuffer );
	ALenum error = alGetError();			// To return as error if needed
	assert( error==AL_NO_ERROR );
	mIsValid = (error==AL_NO_ERROR);
}

Buffer::~Buffer()
{
	// If the object failed to construct, there's nothing to do in the destructor
	if ( !IsValid() )
		return;

	MakeContextCurrent();
	
	// Delete the Raw Buffer
	alGetError();
	alDeleteBuffers( 1, &mRawBuffer );
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );	
	mRawBuffer = NULL;
	
	// Remove the Buffer from the parent PlaybackDevice
	assert( GetParentPlaybackDevice() );
	GetParentPlaybackDevice()->RemoveBuffer( this );
}

bool Buffer::SetBufferData( Format _Format, const ALvoid* _Data, ALsizei _DataSizeInBytes, ALsizei _Frequency )
{
	MakeContextCurrent();
	alGetError();

/*ALuint buffer = GetRawBuffer();
bool isBuffer = alIsBuffer( buffer );
if ( !isBuffer )
{
	printf("err\n");
}*/

	alBufferData( GetRawBuffer(), (ALenum)_Format, _Data, _DataSizeInBytes, _Frequency );	
	ALenum error = alGetError();
	if ( error!=AL_NO_ERROR )
	{
		//printf("err\n")
	}
	return ( error==AL_NO_ERROR );
}

ALint Buffer::GetFrequency() const
{
	MakeContextCurrent();
	return InternalGetIntValue( AL_FREQUENCY );
}

ALint Buffer::GetSizeInBytes() const
{
	MakeContextCurrent();
	return InternalGetIntValue( AL_SIZE );
}

ALint Buffer::GetNumBitsPerFrame() const
{
	MakeContextCurrent();
	ALint l_Value = InternalGetIntValue( AL_BITS );
	assert( l_Value==8 || l_Value==16 );
	return l_Value;
}

ALint Buffer::GetNumChannels() const
{
	MakeContextCurrent();
	ALint l_Value = InternalGetIntValue( AL_CHANNELS );
	assert( l_Value==1 || l_Value==2 );
	return l_Value;
}

void Buffer::MakeContextCurrent() const
{
	GetParentPlaybackDevice()->MakeContextCurrent();
}

ALint Buffer::InternalGetIntValue( ALenum _ParameterName ) const
{
	ALint l_Value = 0;
	alGetError(); 
	alGetBufferi( GetRawBuffer(), _ParameterName, &l_Value ); 
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return l_Value;
}

bool Buffer::InternalSetIntValue( ALenum _ParameterName, ALint _Value ) const
{
	alGetError(); 
	alBufferi( GetRawBuffer(), _ParameterName, _Value ); 
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return ( error==AL_NO_ERROR );
}



}