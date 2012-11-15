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
	static const long MINOR = 10;
	static const long BUILD = 2115;
	static const long REVISION = 11779;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4223;
	#define RC_FILEVERSION 2,10,2115,11779
	#define RC_FILEVERSION_STRING "2, 10, 2115, 11779\0"
	static const char FULLVERSION_STRING[] = "2.10.2115.11779";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 15;
	

}
#endif //VERSION_H
