#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 3;
	static const long MINOR = 8;
	static const long BUILD = 3049;
	static const long REVISION = 16704;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 5960;
	#define RC_FILEVERSION 3,8,3049,16704
	#define RC_FILEVERSION_STRING "3, 8, 3049, 16704\0"
	static const char FULLVERSION_STRING[] = "3.8.3049.16704";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 49;
	

}
#endif //VERSION_H
