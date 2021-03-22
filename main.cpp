#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include "replace.h"
#include "getfiles.h"
#include "ThreadPool.h"

std::mutex m_mutex;

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

void work(std::vector<std::string> files, std::string replace)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    int size = files.size();
    for (int i = 0; i < size; i++)
    {
        std::cout << files[i].c_str() << std::endl;
        Replace r(replace, files[i]);
        r.work();
    }
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
    
    // 获取目录中的全部xml文件的路径
    getFiles(filePath, files);
    
    ThreadPool threads(files);
    threads.ThreadWork(work, replace);
    
	return 0;
}

// g++ .\main.cpp .\replace.cpp -o main
