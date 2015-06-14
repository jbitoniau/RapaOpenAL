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
