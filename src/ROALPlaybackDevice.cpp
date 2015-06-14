#include "ROALPlaybackDevice.h"

#include <assert.h>
#include <algorithm>
#include "ROALRoot.h"
#include "ROALContext.h"
#include "ROALBuffer.h"

namespace ROAL
{

PlaybackDevice::PlaybackDevice( Root* _ParentRoot, const std::string _Name )
	: Device( _ParentRoot, _Name )
{
	alGetError();
	ALCdevice* l_RawDevice = alcOpenDevice( _Name.c_str() );		
	assert( l_RawDevice );
	SetRawDevice( l_RawDevice );
	ALenum error = alGetError();			// To return as error if needed
	assert( error==AL_NO_ERROR );
	SetIsValid( error==AL_NO_ERROR );
}

PlaybackDevice::~PlaybackDevice()
{
	// If the object failed to construct, there's nothing to do in the destructor
	if ( !IsValid() )
		return;

	// Delete the Buffers
	Buffers l_Buffers = mBuffers;
	Buffers::iterator itrBuffers;
	for ( itrBuffers=l_Buffers.begin(); itrBuffers!=l_Buffers.end(); ++itrBuffers )
		delete (*itrBuffers).second;
	assert( mBuffers.empty() );

	// Delete the Contexts
	Contexts l_Contexts = mContexts;
	Contexts::iterator itrContexts;
	for ( itrContexts=l_Contexts.begin(); itrContexts!=l_Contexts.end(); ++itrContexts )
		delete (*itrContexts);
	assert( mContexts.empty() );

	// Close the Raw Device
	alGetError();
	ALCboolean ret = alcCloseDevice( GetRawDevice() );	
	SetRawDevice( NULL );
	ALenum error = alGetError();
	assert( error==AL_NO_ERROR );
	assert( ALC_TRUE==ret );

	// Remove this Device from its parent Root
	Root* l_Root = GetParentRoot();
	assert( l_Root );
	l_Root->RemovePlaybackDevice( this );
}

Context* PlaybackDevice::CreateContext()
{
	Context* l_Context = new Context( this );
	if ( l_Context->IsValid() )
	{
		mContexts.push_back( l_Context );
	}	
	else
	{	
		delete l_Context;
		l_Context = NULL;
	}
	return l_Context;
}

Buffer* PlaybackDevice::CreateBuffer()
{
	Buffer* l_Buffer = new Buffer( this );
	if ( l_Buffer->IsValid() )
	{
		mBuffers.insert( std::make_pair(l_Buffer->GetRawBuffer(), l_Buffer) );
	}
	else
	{
		delete l_Buffer;
		l_Buffer = NULL;
	}
	return l_Buffer;
}

void PlaybackDevice::RemoveContext( Context* _Context )
{
	Contexts::iterator itr;
	for ( itr=mContexts.begin(); itr!=mContexts.end(); ++itr )
	{
		if ( (*itr)==_Context )
		{
			mContexts.erase( itr );
			return;
		}
	}
	assert(false);
}

/*
	Buffers in a PlaybackDevice can't be created/manipulated if there's no context
	This method is called by the Buffer methods to ensure the appropriate context
	is made current.
	Any context of the PlaybackDevice the Buffer is associated with will do it seems.
*/

bool PlaybackDevice::MakeContextCurrent() const
{
	// Can't make any of this device's context current, it has no context!
	assert( !GetContexts().empty() );
	if ( GetContexts().empty() )
		return false;

	Context* l_CurrentContext = Context::GetCurrentContext();
	if ( l_CurrentContext )
	{
		if ( l_CurrentContext->GetParentPlaybackDevice()==this )
			return true;
	}
		
	Context* l_FirstContext = (*GetContexts().begin());
	bool ret = Context::MakeContextCurrent( l_FirstContext );
	assert( ret );
	return ret;	
}

/*
Context* PlaybackDevice::GetContextFromRawContext( ALCcontext* _RawContext )
{
	Context* l_Context = NULL;
	Contexts::iterator itr;
	for ( itr=mContexts.begin(); itr!=mContexts.end(); ++itr )
	{
		if ( (*itr)->GetRawContext()==_RawContext )
		{
			l_Context = (*itr);
			break;
		}
	}
	return l_Context;
}*/

void PlaybackDevice::RemoveBuffer( Buffer* _Buffer )
{
	Buffers::iterator itr;
	for ( itr=mBuffers.begin(); itr!=mBuffers.end(); ++itr )
	{
		if ( (*itr).second==_Buffer )
		{
			mBuffers.erase( itr );
			return;
		}
	}
	assert(false);
}
	
Buffer*	PlaybackDevice::GetBufferFromRawBuffer( ALuint _RawBuffer )
{
	Buffer* l_Buffer = NULL;
	Buffers::iterator itr = mBuffers.find( _RawBuffer );
	if ( itr!=mBuffers.end() )
		l_Buffer = (*itr).second;
	return l_Buffer;
}

}