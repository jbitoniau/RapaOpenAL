#pragma once

#include "ROALDevice.h"
#include "ROALBuffer.h"

namespace ROAL
{

class CaptureDevice : public Device
{
public:
	virtual ~CaptureDevice();
		
	void	Start();
	void	Stop();
	ALCint	GetNumFramesCaptured() const;
	void	GetCaptureFrames( ALCvoid* _Buffer, ALCsizei _NumFramesCaptured );
 
protected:
	friend class Root;
	CaptureDevice(	Root* _ParentRoot, const std::string _Name,
					ALCuint _BufferFrequency, Buffer::Format _BufferFormat, ALCsizei _BufferSizeInFrames );
	
	ALCint	InternalGetIntValue( ALenum _ParameterName ) const;
};

}
