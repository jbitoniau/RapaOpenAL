#pragma once 

#include <string>
#ifdef _WIN32
	#include "al.h"
	#include "alc.h"
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

namespace ROAL
{

class Root;

class Device 
{
public:
	std::string GetName() const { return mName; }
	virtual ~Device();

	bool		IsValid() const { return mIsValid; }

	Root*		GetParentRoot() const	{ return mParentRoot; }

protected:
	friend class Root;
	Device( Root* _ParentRoot, const std::string _Name );
	ALCdevice*	GetRawDevice() const { return mRawDevice; }
	void		SetRawDevice( ALCdevice* _RawDevice ) { mRawDevice = _RawDevice; }
	
	void		SetIsValid( bool _IsValid ) { mIsValid=_IsValid; }

private:
	bool		mIsValid;
	Root*		mParentRoot;
	std::string	mName;
	ALCdevice*	mRawDevice;
};

}