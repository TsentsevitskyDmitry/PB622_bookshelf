#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;
const int BACK = -1;

class FSCrawler
{
private:
 	DIR* dir;
	string currentPath;
	dirent* backBath;		
	dirent* emptyPath;	
    vector<dirent*> files;

public:
	FSCrawler();
	bool isDirectory(int index);		
	bool isBack(int index);	
	int filesCount();
	void update();	
	void open(int index);
	char* getName(int index);
	char* getPath(int index);
	char* operator[] (int index);	
};
