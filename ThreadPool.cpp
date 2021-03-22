#include "ThreadPool.h"

ThreadPool::ThreadPool(const std::vector<std::string>& filePaths)
{
    this->filePaths = filePaths;
    threadNum = std::thread::hardware_concurrency() - 1;
    threads.resize(threadNum);
    // std::cout << "ThreadNum: " << threads.size() << std::endl;
}

std::vector<std::string> ThreadPool::VectorSplit(int i)
{
    std::vector<std::string>::const_iterator first = filePaths.begin() + (filePaths.size() / threadNum) * i;
    std::vector<std::string>::const_iterator last;

    if(i == threads.size() - 1)
    {
        last = filePaths.end() - (filePaths.size() / threadNum) * (threadNum - i - 1);
    }
    else
    {
        last = filePaths.begin() + (filePaths.size() / threadNum) * (i + 1);
    }

    std::vector<std::string> vector(first, last);
    return vector;
}

void ThreadPool::ThreadWork(void (*pf)(std::vector<std::string> files, std::string replace), std::string replace)
{

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i] = std::thread((*pf), VectorSplit(i), replace);
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}