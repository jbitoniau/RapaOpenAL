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
