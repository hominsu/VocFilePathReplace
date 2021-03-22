#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class ThreadPool
{
private:
    int threadNum;
    std::vector<std::string> filePaths;
    std::vector<std::thread> threads;

public:
    void ThreadWork(void (*pf)(std::vector<std::string> files, std::string replace), std::string replace);

private:
    std::vector<std::string> VectorSplit(int i);

public:
    ThreadPool(const std::vector<std::string>& filePaths);
};