#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "07";
	static const char YEAR[] = "2015";
	static const char UBUNTU_VERSION_STYLE[] =  "15.07";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 3;
	static const long MINOR  = 9;
	static const long BUILD  = 3190;
	static const long REVISION  = 17368;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6266;
	#define RC_FILEVERSION 3,9,3190,17368
	#define RC_FILEVERSION_STRING "3, 9, 3190, 17368\0"
	static const char FULLVERSION_STRING [] = "3.9.3190.17368";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 90;
	

}
#endif //VERSION_H
