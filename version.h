#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 3;
	static const long MINOR = 9;
	static const long BUILD = 3128;
	static const long REVISION = 17043;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 6131;
	#define RC_FILEVERSION 3,9,3128,17043
	#define RC_FILEVERSION_STRING "3, 9, 3128, 17043\0"
	static const char FULLVERSION_STRING[] = "3.9.3128.17043";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 28;
	

}
#endif //VERSION_H
