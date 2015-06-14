#include "ROALCaptureDevice.h"

#include <assert.h>
#include "ROALRoot.h"

namespace ROAL
{

CaptureDevice::CaptureDevice( Root* _ParentRoot, const std::string _Name, 
							  ALCuint _BufferFrequency, Buffer::Format _BufferFormat, ALCsizei _BufferSizeInFrames )
	:	Device( _ParentRoot, _Name )
{
	alGetError();
	ALCdevice* l_RawDevice = alcCaptureOpenDevice( _Name.c_str(), _BufferFrequency, _BufferFormat, _BufferSizeInFrames );
	assert( l_RawDevice );
	SetRawDevice( l_RawDevice );
	ALenum error = alGetError();			// To return as error if needed
	assert( error==AL_NO_ERROR );
	SetIsValid( error==AL_NO_ERROR );
}

CaptureDevice::~CaptureDevice()
{
	// If the object failed to construct, there's nothing to do in the destructor
	if ( !IsValid() )
		return;

	alGetError();
	ALCboolean ret = alcCaptureCloseDevice( GetRawDevice() );
	SetRawDevice( NULL );
	assert( ret );
	ALenum error = alGetError();			// To return as error if needed
	assert( error==AL_NO_ERROR );

	// Remove this Device from its parent Root
	Root* l_Root = GetParentRoot();
	assert( l_Root );
	l_Root->RemoveCaptureDevice( this );
}

void CaptureDevice::Start()
{
	alGetError();
	alcCaptureStart( GetRawDevice() );
	ALenum error = alGetError();			// To return as error if needed
	assert( error==AL_NO_ERROR );
}

void CaptureDevice::Stop()
{
	alGetError();
	alcCaptureStop( GetRawDevice() );
	ALenum error = alGetError();			// To return as error if needed
	assert( error==AL_NO_ERROR );
}

ALCint CaptureDevice::GetNumFramesCaptured() const
{
	return InternalGetIntValue( ALC_CAPTURE_SAMPLES );
}

void CaptureDevice::GetCaptureFrames( ALCvoid* _Buffer, ALCsizei _NumFramesCaptured )
{
	assert( _Buffer );
	if ( !_Buffer )
		return;
	ALenum error = alGetError();			// To return as error if needed
	alcCaptureSamples( GetRawDevice(), _Buffer, _NumFramesCaptured );
	assert( error==AL_NO_ERROR );
}

ALCint CaptureDevice::InternalGetIntValue( ALenum _ParameterName ) const
{
	ALCint l_Value = 0;
	alGetError(); 
	alcGetIntegerv( GetRawDevice(), _ParameterName, sizeof(ALCint), &l_Value ); 
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	return l_Value;
}

}
