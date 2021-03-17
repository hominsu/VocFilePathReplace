#include <iostream>
#include <vector>
#include <string>
#include <io.h>

void getFiles(std::string path, std::vector<std::string>& files)
{
	long hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) !=  -1)
	{
		do
		{
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
				{
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
                std::string s = fileinfo.name;
                size_t lastPos = s.find_last_of(".");
                if(strcmp(s.substr(lastPos + 1).c_str(), "xml") == 0)
                {
                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					//files.push_back(p.assign(path).append(fileinfo.name));
                }
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}
