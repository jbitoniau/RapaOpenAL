#include "ROALListener.h"

#include "ROALContext.h"

namespace ROAL
{

Listener::Listener( Context* _ParentContext )
	:	mParentContext(_ParentContext)
{
	// call MakeContextCurrent() if things need to be done here...
}

void Listener::MakeContextCurrent() const
{
	GetParentContext()->MakeContextCurrent();
}

}