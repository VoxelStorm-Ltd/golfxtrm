#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 2;
	static const long BUILD = 274;
	static const long REVISION = 1552;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 468;
	#define RC_FILEVERSION 1,2,274,1552
	#define RC_FILEVERSION_STRING "1, 2, 274, 1552\0"
	static const char FULLVERSION_STRING[] = "1.2.274.1552";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 74;
	

}
#endif //VERSION_H
