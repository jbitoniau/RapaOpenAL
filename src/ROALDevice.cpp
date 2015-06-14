#include "ROALDevice.h"

#include <assert.h>

namespace ROAL
{

Device::Device( Root* _ParentRoot, const std::string _Name )
	:	mIsValid(false),
		mParentRoot(_ParentRoot),
		mName(_Name), 
		mRawDevice(NULL)
{
	assert( mParentRoot );
}

Device::~Device()
{
}


}