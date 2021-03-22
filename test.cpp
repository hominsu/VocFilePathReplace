#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex m_mutex;

class ThreadPool
{
private:
    int threadNum;
    std::vector<std::string> filePaths;
    std::vector<std::thread> threads;

public:
    void ThreadWork(void (*pf)(std::vector<std::string> files, int threads));

private:
    std::vector<std::string> VectorSplit(int i);

public:
    ThreadPool(const std::vector<std::string>& filePaths);
};

ThreadPool::ThreadPool(const std::vector<std::string>& filePaths)
{
    this->filePaths = filePaths;
    threadNum = std::thread::hardware_concurrency() - 1;
    threads.resize(threadNum);
    std::cout << "ThreadNum: " << threads.size() << std::endl;
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

void ThreadPool::ThreadWork(void (*pf)(std::vector<std::string> files, int threads))
{

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i] = std::thread((*pf), VectorSplit(i), i + 1);
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void work(std::vector<std::string> filePaths, int threads)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (int i = 0; i < filePaths.size(); i++)
    {
        std::cout << "Thread " << threads << ": " << filePaths[i] << std::endl;
    }
}

int main(void)
{
    std::vector<std::string> filePaths;
    for (int i = 0; i < 64; i++)
    {
        filePaths.push_back(std::to_string(i + 1));
    }
    ThreadPool threads(filePaths);
    threads.ThreadWork(work);
}