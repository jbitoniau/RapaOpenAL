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
#include "ROALTest.h"

#include <stdio.h>
#include <assert.h>
#include <string>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN 
	#define NOMINMAX 
	#include <windows.h>
#else
	#include <time.h>   
#endif

#include "ROALRoot.h"
#include "ROALPlaybackDevice.h"
#include "ROALContext.h"
#include "ROALBuffer.h"
#include "ROALStaticSource.h"
#include "ROALStreamingSource.h"
#include "ROALCaptureDevice.h"

using namespace ROAL;

#ifdef _WIN32
typedef unsigned long long int u64;
static u64 initialTickCount = 0xffffffffffffffffUL;
float GetTimeAsTicks()
{
	u64 ret;
	LARGE_INTEGER l;
	QueryPerformanceCounter(&l);
	ret = l.QuadPart;
	if ( initialTickCount==0xffffffffffffffffUL )
		initialTickCount = ret;
	ret -= initialTickCount;

	float r = (float)ret / 1000.f;
	return r;
}
#else
float GetTimeAsTicks()
{
	return 0.f;
}

#endif

void TestOpenALO::Test( const char* _FileName, const char* _OutputFileName )
{
	Root* l_Root = new Root();
	printf("Testing device enumeration\n");
	TestDeviceEnumeration( l_Root );
	printf("\n");
	delete l_Root;
	l_Root = NULL;

	l_Root = new Root();

	printf("Testing static source\n");
	TestStaticSource( l_Root, _FileName );
	printf("\n");
	
	printf("Testing streaming source\n");
	TestStreamingSource( l_Root, _FileName );
	printf("\n");

	printf("Testing capture\n");
	TestCapture( l_Root, _OutputFileName );
	printf("\n");

	delete l_Root;
	l_Root = NULL;
}

void TestOpenALO::Sleep( unsigned int milliseconds )
{
#ifdef _WIN32
	::Sleep( milliseconds );
#else
	struct timespec l_TimeSpec;
	l_TimeSpec.tv_sec = milliseconds / 1000;
	l_TimeSpec.tv_nsec = (milliseconds % 1000) * 1000000;
	struct timespec l_Ret;
	nanosleep(&l_TimeSpec,&l_Ret);
#endif
}

bool TestOpenALO::ReadBufferDatas( const std::string& _FileName, unsigned int _NbBytesPerBufferData, std::vector<char*>& _BufferDatas )
{
	FILE* l_File = fopen( _FileName.c_str(), "rb" );
	if ( !l_File )
		return false;

	int l_Ret = 0;
	do
	{
		char* l_BufferData = new char[_NbBytesPerBufferData];
		l_Ret = fread( l_BufferData, _NbBytesPerBufferData, 1, l_File );
		if ( l_Ret==1 )
			_BufferDatas.push_back( l_BufferData );
	}
	while ( l_Ret==1 );

	fclose( l_File );
	return true;
}

bool TestOpenALO::WriteBufferDatas( const std::string& _FileName, unsigned int _NbBytesPerBufferData, std::vector<char*>& _BufferDatas )
{
	FILE* l_File = fopen( _FileName.c_str(), "wb" );
	if ( !l_File )
		return false;

	for ( std::size_t i=0; i<_BufferDatas.size(); ++i )
	{
		char* l_BufferData = _BufferDatas[i];
		int l_Ret = fwrite( l_BufferData, _NbBytesPerBufferData, 1, l_File );
		assert( l_Ret == 1 );
	}

	fclose( l_File );
	return true;
}


void TestOpenALO::DeleteBufferDatas( std::vector<char*>& _BufferDatas )
{
	for ( std::size_t i=0; i<_BufferDatas.size(); ++i )
	{
		delete _BufferDatas[i];
		_BufferDatas[i] = NULL;
	}
	_BufferDatas.clear();
}

void TestOpenALO::TestDeviceEnumeration(Root* _Root)
{
	std::vector<std::string> l_PlaybackDevicesNames = _Root->GetPlaybackDeviceNames();
	printf("Playback devices:\n");
	for ( std::size_t i=0; i<l_PlaybackDevicesNames.size(); ++i )
	{
		printf( "%d - %s ", i, l_PlaybackDevicesNames[i].c_str() );
		if ( i==0 )
			printf( "[Default]" );
		printf( "\n" );
	}
	printf( "\n" );

	std::vector<std::string> l_CaptureDevicesNames = _Root->GetCaptureDeviceNames();
	printf("Capture devices:\n");
	for ( std::size_t i=0; i<l_CaptureDevicesNames.size(); ++i )
	{
		printf( "%d - %s ", i, l_CaptureDevicesNames[i].c_str() );
		if ( i==0 )
			printf( "[Default]" );
		printf( "\n" );
	}
	printf( "\n" );
}

void TestOpenALO::TestStaticSource( Root* _Root, const std::string& _FileName )
{
	bool ret = false;
	unsigned int NumBytesPerFrame = 2;
	unsigned int NumFramesPerBlock = 8000 * 10;
	unsigned int NumBytesPerBlock = NumBytesPerFrame * NumFramesPerBlock;

	printf("Loading buffer data\n");
	std::vector<char*> l_BufferDatas;
	ret = ReadBufferDatas( _FileName, NumBytesPerBlock, l_BufferDatas );
	if (!ret)
	{
		printf("Failed to load data\n");
		return;
	}
	
	printf("Creating device\n");
	PlaybackDevice* l_PlaybackDevice = NULL;
	std::vector<std::string> l_PlaybackDevicesNames = _Root->GetPlaybackDeviceNames();
	if ( l_PlaybackDevicesNames.size()==0 )
	{
		printf("No device to create!\n");
		return;
	}
	l_PlaybackDevice = _Root->CreatePlaybackDevice( l_PlaybackDevicesNames[0] );
	assert( l_PlaybackDevice );	

	printf("Creating context\n");
	Context* l_Context = l_PlaybackDevice->CreateContext();
	assert( l_Context ); 

	printf("Creating buffer\n");
	Buffer* l_Buffer = l_PlaybackDevice->CreateBuffer();
	assert( l_Buffer );	
	
	printf("Setting buffer data\n");
	ret = l_Buffer->SetBufferData( Buffer::MONO16, l_BufferDatas[0], NumBytesPerBlock, 8000 );
	assert( ret ); 

	ALint l_Freq = l_Buffer->GetFrequency();
	assert( l_Freq==8000 ); 
	ALint l_NumBitsPerFrame = l_Buffer->GetNumBitsPerFrame();
	assert( l_NumBitsPerFrame==16 ); 
	ALint l_NumChannels = l_Buffer->GetNumChannels();
	assert( l_NumChannels==1 ); 

	printf("Creating static source\n");
	StaticSource* l_StaticSource = l_Context->CreateStaticSource();		
	assert( l_StaticSource ); 
	assert( l_StaticSource->GetState()==Source::INITIAL ); 
	
	printf("Playing source (no effect)\n");
	ret = l_StaticSource->Play();				
	assert( ret );
	assert( l_StaticSource->GetState()==Source::INITIAL ); 
	
	printf("Pausing source (no effect)\n");
	ret = l_StaticSource->Pause();				
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::INITIAL ); 
	
	printf("Stopping source (no effect)\n");
	ret = l_StaticSource->Stop();				
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::INITIAL ); 
	
	printf("Rewinding source (no effect)\n");
	ret = l_StaticSource->Rewind();				// No buffer attached
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::INITIAL ); 

	printf("Attaching buffer to source\n");
	ret = l_StaticSource->AttachBuffer( l_Buffer );
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::INITIAL ); 

	printf("Playing source\n");
	ret = l_StaticSource->Play();
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	Sleep(3000);

	printf("Pausing source\n");
	ret = l_StaticSource->Pause();
	assert( ret );	
	assert( l_StaticSource->GetState()==Source::PAUSED ); 
	Sleep(2000);
	
	printf("Playing source\n");
	ret = l_StaticSource->Play();
	assert( ret );	
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	Sleep(3000);

	printf("Stopping source\n");
	ret = l_StaticSource->Stop();
	assert( ret );
	assert( l_StaticSource->GetState()==Source::STOPPED ); 
	Sleep(2000);
	
	printf("Playing source\n");
	ret = l_StaticSource->Play();
	assert( ret );
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	Sleep(3000);

	printf("Rewinding source\n");
	ret = l_StaticSource->Rewind();
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::INITIAL ); 
	Sleep(2000);

	printf("Playing source\n");
	ret = l_StaticSource->Play();
	assert( ret ); 	
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	Sleep(3000);

	printf("Playing source\n");
	ret = l_StaticSource->Play();
	assert( ret );	
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	Sleep(3000);

	printf("Creating buffer 2\n");
	Buffer* l_Buffer2 = l_PlaybackDevice->CreateBuffer();
	assert( l_Buffer2 ); 

	printf("Setting buffer 2 data\n");
	ret = l_Buffer2->SetBufferData( Buffer::MONO16, l_BufferDatas[1], NumBytesPerBlock, 8000 );
	assert( ret ); 

	printf("Attaching buffer 2 to source (fail)\n");
	ret = l_StaticSource->AttachBuffer( l_Buffer2 );
//	assert( !ret );      // is true on iOS
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	
	printf("Stopping source\n");
	ret = l_StaticSource->Stop();
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::STOPPED ); 
	Sleep(2000);

	printf("Attaching buffer 2 to source\n");
	ret = l_StaticSource->AttachBuffer( l_Buffer2 );
	assert( ret );
	assert( l_StaticSource->GetState()==Source::STOPPED ); 

	printf("Playing source\n");
	ret = l_StaticSource->Play();
	assert( ret );
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	Sleep(3000);

	printf("Detaching buffer 2 from source (fail)\n");
	ret = l_StaticSource->DetachBuffer();
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
//	assert( !ret );         // is true on iOS

	printf("Stopping source\n");
	ret = l_StaticSource->Stop();
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::STOPPED ); 
	Sleep(2000);
	
	printf("Detaching buffer 2 from source\n");
	ret = l_StaticSource->DetachBuffer();
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::STOPPED ); 
	
	printf("Playing source (no effect)\n");
	ret = l_StaticSource->Play();				
	assert( ret ); 
	assert( l_StaticSource->GetState()==Source::STOPPED ); 
	
	printf("Attaching buffer 1 to source\n");
	ret = l_StaticSource->AttachBuffer( l_Buffer );
	assert( ret );
	assert( l_StaticSource->GetState()==Source::STOPPED ); 

	printf("Playing source\n");
	ret = l_StaticSource->Play();
	assert( l_StaticSource->GetState()==Source::PLAYING ); 
	assert( ret );
	Sleep(3000);

	ret = l_StaticSource->Stop();
	assert( ret );

	ret = l_StaticSource->DetachBuffer();
	assert( ret );

	printf("Deleting device\n");
	delete l_PlaybackDevice;
	l_PlaybackDevice = NULL;

	printf("Deleting buffer data\n");
	DeleteBufferDatas( l_BufferDatas );
}

void TestOpenALO::TestStreamingSource( Root* _Root, const std::string& _FileName )
{
	bool ret = false;
	unsigned int NumBytesPerFrame = 2;
	unsigned int NumFramesPerBlock = 512;
	unsigned int NumBytesPerBlock = NumBytesPerFrame * NumFramesPerBlock;

	printf("Loading buffer data\n");
	std::vector<char*> l_BufferDatas;
	ret = ReadBufferDatas( _FileName, NumBytesPerBlock, l_BufferDatas );
	if (!ret)
	{
		printf("Failed to load data\n");
		return;
	}
	
	printf("Creating device\n");
	PlaybackDevice* l_PlaybackDevice = NULL;
	std::vector<std::string> l_PlaybackDevicesNames = _Root->GetPlaybackDeviceNames();
	if ( l_PlaybackDevicesNames.size()==0 )
	{
		printf("No device to create!\n");
		return;
	}
	l_PlaybackDevice = _Root->CreatePlaybackDevice( l_PlaybackDevicesNames[0] );
	assert( l_PlaybackDevice );	

	printf("Creating context\n");
	Context* l_Context = l_PlaybackDevice->CreateContext();
	assert( l_Context ); 

	printf("Creating streaming source\n");
	StreamingSource* l_StreamingSource = l_Context->CreateStreamingSource();		
	assert( l_StreamingSource ); 
	assert( l_StreamingSource->GetState()==Source::INITIAL ); 

	// Creating buffers
	ALuint l_NumBuffersToQueue = 3;
	std::vector<Buffer*> l_BuffersReady;
	for ( ALuint i=0; i<l_NumBuffersToQueue; ++i )
	{
		printf("Creating buffer\n");
		Buffer* l_Buffer = l_PlaybackDevice->CreateBuffer();
		assert( l_Buffer );	
		l_BuffersReady.push_back( l_Buffer );
	}
	
	unsigned int l_BufferIndex = 0;
	do
	{
		/*if ( l_BufferIndex==10 )
		{
			printf("Triggering underrun\n");
			Sleep( 4000 );
		}*/
		{
			l_StreamingSource->GetParentContext()->Process();
			ALuint l_NumBuffersQueued = l_StreamingSource->GetNumBuffersQueued();
			ALuint l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
			printf("%0.0f - Process %d/%d\n", GetTimeAsTicks(), l_NumBuffersProcessed, l_NumBuffersQueued );
		}
		// We first unqueue the buffers that have been processed and that we can recycle
		ALint l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
		for ( int i=0; i<l_NumBuffersProcessed; ++i )
		{
			ALuint l_NumBuffersQueued = l_StreamingSource->GetNumBuffersQueued();
			ALuint l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
			printf("%0.0f - Unqueuing buffer %d/%d\n", GetTimeAsTicks(), l_NumBuffersProcessed, l_NumBuffersQueued );
			Buffer* l_Buffer = l_StreamingSource->UnqueueBuffer();
			assert( l_Buffer );
			l_BuffersReady.push_back( l_Buffer );
		}
		
		// We recycle the buffers by setting new data and re-queuing them
		for ( unsigned int i=0; i<l_BuffersReady.size(); ++i )
		{
			Buffer* l_Buffer = l_BuffersReady[i];
					
			ALuint l_NumBuffersQueued = l_StreamingSource->GetNumBuffersQueued();
			ALuint l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
		//	printf("%0.0f - Setting buffer data %d/%d\n", GetTimeAsTicks(), l_NumBuffersProcessed, l_NumBuffersQueued );
			ret = l_Buffer->SetBufferData( Buffer::MONO16, l_BufferDatas[l_BufferIndex], NumBytesPerBlock, 8000 );
			l_BufferIndex++;
			assert( ret );

			l_NumBuffersQueued = l_StreamingSource->GetNumBuffersQueued();
			l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
			printf("%0.0f - Queuing buffer %d/%d\n", GetTimeAsTicks(), l_NumBuffersProcessed, l_NumBuffersQueued );
			ret = l_StreamingSource->QueueBuffer( l_Buffer );

			//l_StreamingSource->GetParentContext()->Process();
			l_NumBuffersQueued = l_StreamingSource->GetNumBuffersQueued();
			l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
			//printf("%0.0f - aft Queuing buffer %d/%d\n", GetTimeAsTicks(), l_NumBuffersProcessed, l_NumBuffersQueued );
			
			assert( ret );
		}
		l_BuffersReady.clear();

		// If the source is not playing...		
		if ( l_StreamingSource->GetState()!=Source::PLAYING )
		{	
			// We check how many buffers have been queued
			// And if we've reach full buffering capacity, we play the source
			ALuint l_NumBuffersQueued = l_StreamingSource->GetNumBuffersQueued();
			//ALuint l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
			//printf("Num Queued=%d  Num Processed=%d\n", l_NumBuffersQueued, l_NumBuffersProcessed );
			//printf("%0.0f - 				!!! %d/%d\n", GetTimeAsTicks(), l_NumBuffersProcessed, l_NumBuffersQueued );
		
			if ( l_NumBuffersQueued==l_NumBuffersToQueue )
			{
				ALuint l_NumBuffersQueued = l_StreamingSource->GetNumBuffersQueued();
				ALuint l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
				printf("%0.0f - Playing source %d/%d\n", GetTimeAsTicks(), l_NumBuffersProcessed, l_NumBuffersQueued );
				ret = l_StreamingSource->Play();
				assert( ret );
			}
		}
		Sleep(30);
	}
	while ( l_BufferIndex<l_BufferDatas.size() );
	assert( l_BuffersReady.empty() );

	printf("Waiting for source to finish playing\n");
	do
	{	
		Sleep(10);
		printf(".");
	}
	while ( l_StreamingSource->GetState()!=Source::STOPPED );
	printf("\n");

	ALint l_NumBuffersProcessed = l_StreamingSource->GetNumBuffersProcessed();
	assert( l_NumBuffersProcessed==static_cast<ALint>(l_NumBuffersToQueue) );
	for ( int i=0; i<l_NumBuffersProcessed; ++i )
	{
		printf("Unqueuing buffer\n");
		Buffer* l_Buffer = l_StreamingSource->UnqueueBuffer();
		assert( l_Buffer );

		printf("Deleting buffer %d\n", i);
		delete l_Buffer;
	}
	
	printf("Deleting device\n");
	delete l_PlaybackDevice;
	l_PlaybackDevice = NULL;

	printf("Deleting buffer data\n");
	DeleteBufferDatas( l_BufferDatas );
}

void TestOpenALO::TestCapture( Root* _Root, const std::string& _OutputFileName )
{
	ALCint l_Frequency = 8000;
	Buffer::Format l_Format = Buffer::MONO16;
	unsigned int l_NumBytesPerFrame = 2;
	ALCsizei l_BufferSizeInFrames = 8000 ;
	ALCsizei l_NumBuffers = 3;

	std::vector<char*> l_BufferDatas;

	printf("Creating device\n");
	CaptureDevice* l_CaptureDevice = NULL;
	std::vector<std::string> l_CaptureDevicesNames = _Root->GetCaptureDeviceNames();
	if ( l_CaptureDevicesNames.size()==0 )
	{
		printf("No device to create!\n");
		return;
	}
	l_CaptureDevice = _Root->CreateCaptureDevice( l_CaptureDevicesNames[0], l_Frequency, l_Format, l_BufferSizeInFrames*l_NumBuffers );
	assert( l_CaptureDevice );	

	printf("Starting capture device\n");
	l_CaptureDevice->Start();

	do
	{
		ALCint l_NumFramesCaptured = l_CaptureDevice->GetNumFramesCaptured();
		printf("%d frames captured\n", l_NumFramesCaptured );
		if ( l_NumFramesCaptured>=l_BufferSizeInFrames )
		{
			printf("Getting captured frames as a buffer\n");
			char* l_BufferData = new char[ l_NumBytesPerFrame * l_BufferSizeInFrames];
			l_CaptureDevice->GetCaptureFrames( l_BufferData, l_BufferSizeInFrames );
			l_BufferDatas.push_back( l_BufferData );
		}	
		Sleep(100);
	}
	while ( l_BufferDatas.size()<10 );
	
	printf("Stopping capture device\n");
	l_CaptureDevice->Stop();

	printf("Saving captured buffer data\n");
	bool ret = WriteBufferDatas( _OutputFileName, l_BufferSizeInFrames * l_NumBytesPerFrame, l_BufferDatas );
	assert( ret) ;
	
	printf("Deleting device\n");
	delete l_CaptureDevice;
	l_CaptureDevice = NULL;
}
