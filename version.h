#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 3;
	static const long MINOR = 6;
	static const long BUILD = 2862;
	static const long REVISION = 15727;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 5593;
	#define RC_FILEVERSION 3,6,2862,15727
	#define RC_FILEVERSION_STRING "3, 6, 2862, 15727\0"
	static const char FULLVERSION_STRING[] = "3.6.2862.15727";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 62;
	

}
#endif //VERSION_H
