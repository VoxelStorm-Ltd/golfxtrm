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
	static const long BUILD = 3064;
	static const long REVISION = 16776;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 5995;
	#define RC_FILEVERSION 3,8,3064,16776
	#define RC_FILEVERSION_STRING "3, 8, 3064, 16776\0"
	static const char FULLVERSION_STRING[] = "3.8.3064.16776";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 64;
	

}
#endif //VERSION_H
