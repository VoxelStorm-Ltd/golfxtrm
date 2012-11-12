#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 6;
	static const long BUILD = 621;
	static const long REVISION = 3469;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1161;
	#define RC_FILEVERSION 1,6,621,3469
	#define RC_FILEVERSION_STRING "1, 6, 621, 3469\0"
	static const char FULLVERSION_STRING[] = "1.6.621.3469";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 21;
	

}
#endif //VERSION_H
