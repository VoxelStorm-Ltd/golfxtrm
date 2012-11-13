#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "13";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 1152;
	static const long REVISION = 6425;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2262;
	#define RC_FILEVERSION 2,0,1152,6425
	#define RC_FILEVERSION_STRING "2, 0, 1152, 6425\0"
	static const char FULLVERSION_STRING[] = "2.0.1152.6425";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 52;
	

}
#endif //VERSION_H
