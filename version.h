#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "21";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.05";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 3;
	static const long MINOR = 9;
	static const long BUILD = 3131;
	static const long REVISION = 17063;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 6137;
	#define RC_FILEVERSION 3,9,3131,17063
	#define RC_FILEVERSION_STRING "3, 9, 3131, 17063\0"
	static const char FULLVERSION_STRING[] = "3.9.3131.17063";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 31;
	

}
#endif //VERSION_H
