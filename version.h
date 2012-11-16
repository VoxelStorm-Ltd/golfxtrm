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
	static const long MINOR = 0;
	static const long BUILD = 2257;
	static const long REVISION = 12519;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 4424;
	#define RC_FILEVERSION 3,0,2257,12519
	#define RC_FILEVERSION_STRING "3, 0, 2257, 12519\0"
	static const char FULLVERSION_STRING[] = "3.0.2257.12519";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 57;
	

}
#endif //VERSION_H
