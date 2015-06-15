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

#include <vector>	// JBM!
#include <map>
#include "ROALDevice.h"

namespace ROAL
{

class Context;
class Buffer;

class PlaybackDevice : public Device
{
public:
	typedef std::vector<Context*> Contexts;
	typedef std::map<ALuint, Buffer*> Buffers;
	
	virtual ~PlaybackDevice();

	Context*		CreateContext();			// There can be settings there in theory here...
	const Contexts& GetContexts() const			{ return mContexts; }	
	
	Buffer*			CreateBuffer();
	const Buffers&	GetBuffers() const			{ return mBuffers; }

protected:
	friend class Root;
	PlaybackDevice( Root* _ParentRoot, const std::string _Name );
	
	bool			MakeContextCurrent() const;

	friend class Context;
	void		RemoveContext( Context* _Context );

	friend class Buffer;
	void		RemoveBuffer( Buffer* _Buffer );

	friend class Source;
	friend class StreamingSource;
	Buffer*		GetBufferFromRawBuffer( ALuint _RawBuffer );
	
private:
	Contexts	mContexts;
	Buffers		mBuffers;
};

}
