#include <iostream>
#include <vector>
#include <string>
#include "replace.h"
#include "getfiles.h"


void help()
{
    // 显示帮助
	std::cout << std::endl;
	std::cout << "This tool replaces the file path of the image in the XML file in the dataset" << std::endl << std::endl;
    std::cout << "Usage: " << std::endl;
	std::cout << "\tmain.exe [DataSet Path] [Replace Path]" << std::endl << std::endl;
	std::cout << "For example: " << std::endl;
	std::cout << "\t.\\main.exe C:\\Users\\17740\\Desktop\\DataSet\\ D:\\xyolo\\images\\train\\" 
	<< std::endl << std::endl;
}

int main(int argc, char **argv)
{
    // 显示帮助
	if(argc == 1 || (argc == 2 &&(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)))
    {
        help();
        return 0;
    }

	std::cout << std::endl;

	std::string filePath = argv[1];
    std::string replace = argv[2];
    std::vector<std::string> files;

    // 规范化字符串
    if(filePath.find_last_of("\\") == filePath.length() - 1)
    {
        filePath.pop_back();
    }
    if(replace.find_last_of("\\") < replace.length() - 1)
    {
        replace.push_back('\\');
    }
    
    // 获取目录中的全部xml文件的路径
    getFiles(filePath, files);
    
    // 遍历容器中的文件路径
    int size = files.size();
    for (int i = 0; i < size; i++)
    {
        std::cout << files[i].c_str() << std::endl;
        Replace r(replace, files[i]);
        r.work();
    }
    
	return 0;
}

// g++ .\main.cpp .\replace.cpp -o main
