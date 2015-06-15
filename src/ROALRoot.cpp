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
#include "ROALRoot.h"

#include <assert.h>
#include <algorithm>
#include "ROALPlaybackDevice.h"
#include "ROALCaptureDevice.h"

namespace ROAL
{

Root* Root::mInstance = NULL;

Root::Root()
{
	assert( !mInstance );
	if ( !mInstance )
		mInstance = this;
}

Root::~Root()
{
	// Delete playback devices
	std::vector<PlaybackDevice*> l_PlaybackDevices = mPlaybackDevices;
	for ( std::size_t i=0; i<l_PlaybackDevices.size(); ++i )
		delete l_PlaybackDevices[i];
	assert( mPlaybackDevices.empty() );

	// Delete capture devices
	std::vector<CaptureDevice*> l_CaptureDevices = mCaptureDevices;
	for ( std::size_t i=0; i<l_CaptureDevices.size(); ++i )
		delete l_CaptureDevices[i];
	assert( mCaptureDevices.empty() );

	mInstance = NULL;
}

std::vector<std::string> Root::GetPlaybackDeviceNames()
{
	std::vector<std::string> l_DeviceNames;
	GetDeviceNames( true, l_DeviceNames );
	return l_DeviceNames;
}

std::vector<std::string> Root::GetCaptureDeviceNames()
{
	std::vector<std::string> l_DeviceNames;
	GetDeviceNames( false, l_DeviceNames );
	return l_DeviceNames;
}

PlaybackDevice* Root::CreatePlaybackDevice( const std::string& _Name )
{
	PlaybackDevice* l_PlaybackDevice = new PlaybackDevice( this, _Name );
	if ( l_PlaybackDevice->IsValid() )
	{
		mPlaybackDevices.push_back( l_PlaybackDevice );
	}
	else
	{
		delete l_PlaybackDevice;
		l_PlaybackDevice = NULL;		
	}	
	return l_PlaybackDevice;
}

CaptureDevice* Root::CreateCaptureDevice( const std::string& _Name, ALCuint _BufferFrequency, Buffer::Format _BufferFormat, ALCsizei _BufferSizeInFrames )
{
	CaptureDevice* l_CaptureDevice = new CaptureDevice( this, _Name, _BufferFrequency, _BufferFormat, _BufferSizeInFrames );
	if ( l_CaptureDevice->IsValid() )
	{
		mCaptureDevices.push_back( l_CaptureDevice );
	}
	else
	{
		delete l_CaptureDevice;
		l_CaptureDevice = NULL;		
	}	
	return l_CaptureDevice;
}

bool Root::GetDeviceNames( bool _Playback, std::vector<std::string>& _Names )
{
	_Names.clear();

	// Check whether we can actually enumerate the devices
	if ( alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT")!=AL_TRUE )
		return false;

	// Prepare the parameters for enumeration 
    ALenum l_ParamEnumerateDevices;
    if ( _Playback )
        l_ParamEnumerateDevices = ALC_DEVICE_SPECIFIER;
    else
        l_ParamEnumerateDevices = ALC_CAPTURE_DEVICE_SPECIFIER;
	
    ALenum l_ParamGetDefaultDevice;
    if ( _Playback )
        l_ParamGetDefaultDevice = ALC_DEFAULT_DEVICE_SPECIFIER; 
    else
        l_ParamGetDefaultDevice = ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER;

	// Get default device name 
	const ALCchar* l_RawDefaultDeviceName = alcGetString( NULL, l_ParamGetDefaultDevice );
	bool l_DefaultDeviceNameDefined = false;
	std::string l_DefaultDeviceName;
    if ( l_RawDefaultDeviceName )
	{
		l_DefaultDeviceNameDefined = true;
		l_DefaultDeviceName = l_RawDefaultDeviceName;
	}
    
	// Get devices names
	std::vector<std::string> l_DeviceNames;
	const ALCchar* l_RawDeviceNames = alcGetString( NULL, l_ParamEnumerateDevices );
    if ( !l_RawDeviceNames )
	{
		assert( !l_RawDefaultDeviceName );		// If we couldn't get the devices names, we shouldn't get the default device name!
		return false;
	}

	int i = 0;
	while ( l_RawDeviceNames[i]!=0 )
    {
		// Get the next device name in the list
		std::string l_DeviceName;
        while ( l_RawDeviceNames[i]!=0 )
        {
			l_DeviceName += l_RawDeviceNames[i];
			i++;
		}

		// If the device is the default one, place it first in the list we return
		// otherwise add it at the end of it
		if ( l_DefaultDeviceNameDefined && l_DeviceName==l_DefaultDeviceName )
			l_DeviceNames.insert( l_DeviceNames.begin(), l_DeviceName );
		else
			l_DeviceNames.push_back( l_DeviceName );

		i++;
    }    

	// Here we know if a default device is defined by OpenAL with l_DefaultDeviceNameDefined 
	// But we don't carry this information out of the method. The developper will have to assume
	// the first device name is the default one whether it's really defined or not by OpenAL
	_Names = l_DeviceNames;
	return true;
}

void Root::RemovePlaybackDevice( PlaybackDevice* _PlaybackDevice )
{
	std::vector<PlaybackDevice*>::iterator itr;
	itr = std::find( mPlaybackDevices.begin(), mPlaybackDevices.end(), _PlaybackDevice );
	assert( itr!=mPlaybackDevices.end() );
	mPlaybackDevices.erase( itr );
}

void Root::RemoveCaptureDevice( CaptureDevice* _CaptureDevice )
{
	std::vector<CaptureDevice*>::iterator itr;
	itr = std::find( mCaptureDevices.begin(), mCaptureDevices.end(), _CaptureDevice );
	assert( itr!=mCaptureDevices.end() );
	mCaptureDevices.erase( itr );
}

}
