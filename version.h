#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "15";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 6;
	static const long BUILD = 1775;
	static const long REVISION = 9920;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3523;
	#define RC_FILEVERSION 2,6,1775,9920
	#define RC_FILEVERSION_STRING "2, 6, 1775, 9920\0"
	static const char FULLVERSION_STRING[] = "2.6.1775.9920";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 75;
	

}
#endif //VERSION_H
