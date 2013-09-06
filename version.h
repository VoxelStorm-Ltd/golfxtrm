#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.09";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 3;
	static const long MINOR = 9;
	static const long BUILD = 3168;
	static const long REVISION = 17240;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 6180;
	#define RC_FILEVERSION 3,9,3168,17240
	#define RC_FILEVERSION_STRING "3, 9, 3168, 17240\0"
	static const char FULLVERSION_STRING[] = "3.9.3168.17240";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 68;
	

}
#endif //VERSION_H
