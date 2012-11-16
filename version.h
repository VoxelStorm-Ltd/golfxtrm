#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "16";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 3;
	static const long MINOR = 4;
	static const long BUILD = 2664;
	static const long REVISION = 14643;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 5226;
	#define RC_FILEVERSION 3,4,2664,14643
	#define RC_FILEVERSION_STRING "3, 4, 2664, 14643\0"
	static const char FULLVERSION_STRING[] = "3.4.2664.14643";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 64;
	

}
#endif //VERSION_H
