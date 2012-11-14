#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 4;
	static const long BUILD = 1509;
	static const long REVISION = 8441;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3003;
	#define RC_FILEVERSION 2,4,1509,8441
	#define RC_FILEVERSION_STRING "2, 4, 1509, 8441\0"
	static const char FULLVERSION_STRING[] = "2.4.1509.8441";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 9;
	

}
#endif //VERSION_H
