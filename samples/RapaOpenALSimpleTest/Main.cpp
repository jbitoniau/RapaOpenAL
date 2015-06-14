#include "ROALTest.h"

int main( int /*argc*/, char** /*argv*/ )
{
	std::string filename = "OneToTwenty.raw";
	TestOpenALO::Test( filename.c_str(), "Output.raw");
	return 0;
}
