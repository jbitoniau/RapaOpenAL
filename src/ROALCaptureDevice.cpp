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
