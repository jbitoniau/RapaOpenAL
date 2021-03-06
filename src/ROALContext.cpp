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
#include "ROALContext.h"

#include <assert.h>
#include <algorithm>
#include "ROALPlaybackDevice.h"
#include "ROALListener.h"
#include "ROALStaticSource.h"
#include "ROALStreamingSource.h"

namespace ROAL
{

Context* Context::mCurrentContext = NULL;

Context::Context( PlaybackDevice* _ParentPlaybackDevice )
	:	mIsValid(false),
		mRawContext( NULL ),
		mParentPlaybackDevice( _ParentPlaybackDevice ),
		mListener( NULL )
{
	alGetError();
	mRawContext = alcCreateContext( mParentPlaybackDevice->GetRawDevice(), NULL );
	assert( mRawContext );
	ALenum error = alGetError();		// To return as error if needed
	assert( error==AL_NO_ERROR );
	mIsValid = (error==AL_NO_ERROR);

	if ( IsValid() )
		mListener = new Listener( this );	
}

Context::~Context()
{
	// If the object failed to construct, there's nothing to do in the destructor
	if ( !IsValid() )
		return;

	// Delete the Sources
	std::vector<Source*> l_Sources = mSources;
	for ( std::size_t i=0; i<l_Sources.size(); ++i )
		delete l_Sources[i];
	assert( mSources.empty() );
	
	// Delete the Listener
	assert( mListener );
	delete mListener;
	mListener = NULL;

	// If the Context is the current one, set no current Context.
	// The next operation happening on a remaining context will 
	// automatically set it as current.
	if ( GetCurrentContext()==this )
	{
		bool ret = MakeContextCurrent(NULL);
		assert( ret );
	}

	// Delete the Raw Context
	alGetError();
	alcDestroyContext( GetRawContext() );
	assert( alGetError()==AL_NO_ERROR );
	mRawContext = NULL;

	// Remove this Context from its parent PlaybackDevice
	assert( GetParentPlaybackDevice() );
	GetParentPlaybackDevice()->RemoveContext( this );
}

void Context::Process()
{
	if ( !IsValid() )
		return;
	
	ALCcontext* l_RawContext = GetRawContext();
	alGetError();
	alcProcessContext( l_RawContext );
	assert( alGetError()==AL_NO_ERROR );
}

void Context::MakeContextCurrent() const
{
	bool ret = MakeContextCurrent( const_cast<Context*>(this) );
	assert( ret );
}

bool Context::MakeContextCurrent( Context* _Context ) 
{
	// Is it already the current context?
	if ( GetCurrentContext()==_Context )
		return true;

	ALCcontext* l_RawContext = NULL;
	if ( _Context )
	{
		l_RawContext = _Context->GetRawContext();
		assert( l_RawContext );
		if ( !l_RawContext )
			return false;	
	}
	
	alGetError();
	ALCboolean ret = alcMakeContextCurrent( l_RawContext );
	assert( alGetError()==AL_NO_ERROR );
	if ( ALC_FALSE==ret )
		return false;

	mCurrentContext = _Context;

	return true;
}

/*
Context* Context::GetCurrentContext()
{
	Context* l_Context = NULL;
	alGetError();
	ALCcontext* l_RawContext = alcGetCurrentContext();
	assert( alGetError()==AL_NO_ERROR );
	if ( l_RawContext )
		l_Context = GetParentPlaybackDevice()->GetContextFromRawContext( l_RawContext );
	return l_Context;
}*/

StaticSource* Context::CreateStaticSource()
{
	StaticSource* l_Source = new StaticSource( this );
	if ( l_Source->IsValid() )
	{
		mSources.push_back( l_Source );
	}
	else
	{
		delete l_Source;
		l_Source = NULL;
	}
	return l_Source;
}

StreamingSource* Context::CreateStreamingSource()
{
	StreamingSource* l_Source = new StreamingSource( this );
	if ( l_Source->IsValid() )
	{
		mSources.push_back( l_Source );
	}
	else
	{
		delete l_Source;
		l_Source = NULL;
	}
	return l_Source;
}

void Context::RemoveSource( Source* _Source )
{
	std::vector<Source*>::iterator itr;
	itr = std::find( mSources.begin(), mSources.end(), _Source );
	assert( itr!=mSources.end() );
	mSources.erase( itr );
}


}