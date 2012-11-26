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
	static const long MINOR = 7;
	static const long BUILD = 2963;
	static const long REVISION = 16224;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 5779;
	#define RC_FILEVERSION 3,7,2963,16224
	#define RC_FILEVERSION_STRING "3, 7, 2963, 16224\0"
	static const char FULLVERSION_STRING[] = "3.7.2963.16224";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 63;
	

}
#endif //VERSION_H
