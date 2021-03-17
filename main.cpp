#include <iostream>
#include <vector>
#include <string>
#include "replace.h"
#include "getfiles.h"


void help()
{
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
	if(argc == 1 || (argc == 2 &&(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)))
    {
        help();
        return 0;
    }

	std::cout << std::endl;

    //std::string filePath = "C:\\Users\\17740\\Desktop\\replace";
    //std::string replace = "D:\\xyolo\\images\\train\\";
	std::string filePath = argv[1];
    std::string replace = argv[2];
    std::vector<std::string> files;
    
    getFiles(filePath, files);
    
    int size = files.size();
    for (int i = 0;i < size;i++)
    {
        std::cout << files[i].c_str() << std::endl;
        Replace r(replace, files[i]);
        r.work();
    }
	return 0;
}

// g++ .\main.cpp .\replace.cpp -o main
