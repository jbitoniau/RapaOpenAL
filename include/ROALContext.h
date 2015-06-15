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
