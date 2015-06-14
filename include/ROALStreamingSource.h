#pragma once

#include "ROALSource.h"

namespace ROAL
{

class StreamingSource : public Source
{
public:
	bool		QueueBuffer( Buffer* _Buffer );
	Buffer*		UnqueueBuffer();
	
protected:
	friend class Context;
	StreamingSource( Context* _ParentContext );
};

}
