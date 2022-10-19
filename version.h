#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2022";
	static const char UBUNTU_VERSION_STYLE[] =  "22.10";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 4;
	static const long MINOR  = 1;
	static const long BUILD  = 3222;
	static const long REVISION  = 17549;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6313;
	#define RC_FILEVERSION 4,1,3222,17549
	#define RC_FILEVERSION_STRING "4, 1, 3222, 17549\0"
	static const char FULLVERSION_STRING [] = "4.1.3222.17549";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 22;
	

}
#endif //VERSION_H
