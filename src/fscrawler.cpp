#include "fscrawler.h"

FSCrawler::FSCrawler()
{
	backBath = new dirent;
	strcpy(backBath->d_name, "Back");
	backBath->d_type = DT_DIR;
	backBath->d_ino = BACK;
	emptyPath = new dirent;
	strcpy(emptyPath->d_name, "");
	files.push_back(emptyPath);	
	files.push_back(backBath);

	currentPath = ".";
	update();
}

void FSCrawler::open(int index)
{
	if(index < 0 || index >= files.size() || !isDirectory(index))
		return;

	else if(files[index]->d_ino == BACK)
	{	
		int pos = currentPath.find_last_of('/');
		currentPath = currentPath.substr(0, currentPath.length() - pos - 1);
	}
	else
	{
		currentPath += '/';
		currentPath += string(files[index]->d_name);
	}
 
	files.clear();
	files.push_back(backBath);

	update();
}

void FSCrawler::update()
{
	if(dir) closedir(dir);
	dir = opendir(currentPath.c_str());
    if(!dir)
      cout << "Error opening directory" << endl;
	else
	{
		dirent* item;
		while((item = readdir(dir)) != NULL)
		{
			if(!strcmp(item->d_name, ".") || !strcmp(item->d_name, ".."))
				continue;
			cout << item->d_name << " = ";			
			cout << (item->d_type == DT_DIR ? "dir" : "file") << endl;
			files.push_back(item);
		}
	}
	files.push_back(emptyPath);
}

char* FSCrawler::getName(int index)
{
	if(index < 0 || index >= files.size()) return "";
	return (char*)string(files[index]->d_name).c_str();
}

int FSCrawler::filesCount()
{
	return files.size() - 1;
}

bool FSCrawler::isDirectory(int index)
{
	if(index < 0 || index >= files.size()) return false;
	return files[index]->d_type == DT_DIR;
}

bool FSCrawler::isBack(int index)
{
	if(index < 0 || index >= files.size()) return false;
	return files[index]->d_ino == BACK;
}

char* FSCrawler::getPath(int index)
{
	static string path;
	path = currentPath;
	path += files[index] + '/';
	return path.c_str();
}


char* FSCrawler::operator[] (int index)
{
	return files[index]->d_name;
}
