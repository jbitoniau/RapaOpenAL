#pragma once

#include <vector>
#ifdef _WIN32
	#include "al.h"
	#include "alc.h"
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

namespace ROAL
{

class PlaybackDevice;
class Listener;
class Source;
class StaticSource;
class StreamingSource;

class Context
{
public:
	virtual ~Context();

	bool					IsValid() const { return mIsValid; }

	void					Process();

	PlaybackDevice*			GetParentPlaybackDevice() const	{ return mParentPlaybackDevice; }
	Listener*				GetListener() const				{ return mListener; }
	
	StaticSource*			CreateStaticSource();
	StreamingSource*		CreateStreamingSource();	

protected:
	friend class PlaybackDevice;
	Context( PlaybackDevice* _ParentPlaybackDevice );
	ALCcontext*				GetRawContext() const { return mRawContext; }
	
	friend class Source;
	void					RemoveSource( Source* _Source );

	friend class Listener;
	static Context*			GetCurrentContext() { return mCurrentContext; }
	void					MakeContextCurrent() const;
	static bool				MakeContextCurrent( Context* _Context );
		
private:
	static Context*			mCurrentContext;
	bool					mIsValid;
	ALCcontext*				mRawContext;
	PlaybackDevice*			mParentPlaybackDevice;
	Listener*				mListener;
	std::vector<Source*>	mSources;
};

}
