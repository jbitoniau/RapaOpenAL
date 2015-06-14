#pragma once

#include <string>
#include "ROALRoot.h"

class TestOpenALO
{
public:
	static void Test( const char* _InputFileName, const char* _OutputFileName );
	static void TestDeviceEnumeration( ROAL::Root* _Root);
	static void TestStaticSource( ROAL::Root* _Root, const std::string& _FileName );
	static void TestStreamingSource( ROAL::Root* _Root, const std::string& _FileName );
	static void TestCapture( ROAL::Root* _Root, const std::string& _OutputFileName );

protected:
	static void Sleep( unsigned int milliseconds );
	static bool ReadBufferDatas( const std::string& _FileName, unsigned int _NbBytesPerBufferData, std::vector<char*>& _BufferDatas );
	static bool WriteBufferDatas( const std::string& _FileName, unsigned int _NbBytesPerBufferData, std::vector<char*>& _BufferDatas );
	static void DeleteBufferDatas( std::vector<char*>& _BufferDatas );
};
