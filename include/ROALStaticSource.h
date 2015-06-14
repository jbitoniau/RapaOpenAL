#pragma once

#include "ROALSource.h"

namespace ROAL
{

class StaticSource : public Source
{
public:
	bool AttachBuffer( Buffer* _Buffer );
	bool DetachBuffer();

protected:
	friend class Context;
	StaticSource( Context* _ParentContext );
};

}
