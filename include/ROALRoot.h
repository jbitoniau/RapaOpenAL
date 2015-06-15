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
#pragma once

#include <string>
#include <vector>
#ifdef _WIN32
	#include "al.h"
	#include "alc.h"
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif
#include "ROALBuffer.h"

namespace ROAL
{

class PlaybackDevice;
class CaptureDevice;

class Root
{
public:
	Root();
	virtual ~Root();
	
	std::vector<std::string>	GetPlaybackDeviceNames();
	std::vector<std::string>	GetCaptureDeviceNames();
	
	PlaybackDevice*				CreatePlaybackDevice( const std::string& _Name );
	CaptureDevice*				CreateCaptureDevice( const std::string& _Name, ALCuint _BufferFrequency, Buffer::Format _BufferFormat, ALCsizei _BufferSizeInFrames );

protected:
	static bool					GetDeviceNames( bool _Playback, std::vector<std::string>& _Names ); 

	friend class PlaybackDevice;
	void						RemovePlaybackDevice( PlaybackDevice* _PlaybackDevice );	
	
	friend class CaptureDevice;
	void						RemoveCaptureDevice( CaptureDevice* _CaptureDevice );	

private:
	static Root*					mInstance;
	std::vector<PlaybackDevice*>	mPlaybackDevices;
	std::vector<CaptureDevice*>		mCaptureDevices;
};

}
