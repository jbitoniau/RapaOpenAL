#pragma once

namespace ROAL
{
    
class Context;

class Listener
{
public:
	Context* GetParentContext() const { return mParentContext; }

protected:
	friend class Context;
	Listener( Context* _ParentContext );

	void MakeContextCurrent() const;		// To call before each operation on the Listener

private:
	Context* mParentContext;
};

}
