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
