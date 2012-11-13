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
	static const long MAJOR = 1;
	static const long MINOR = 8;
	static const long BUILD = 854;
	static const long REVISION = 4753;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1600;
	#define RC_FILEVERSION 1,8,854,4753
	#define RC_FILEVERSION_STRING "1, 8, 854, 4753\0"
	static const char FULLVERSION_STRING[] = "1.8.854.4753";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 54;
	

}
#endif //VERSION_H
