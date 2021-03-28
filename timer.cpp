#include "timer.h"

std::mutex sleepMutex;
std::condition_variable condition;

void loader(bool& isRunning, int timeout, const size_t&fileSize, const size_t& sentBytes, const std::string& str)
{
    size_t status;

    std::unique_lock<std::mutex> sleepLock(sleepMutex);
    while(isRunning)
    {
        condition.wait_for(sleepLock, std::chrono::milliseconds(timeout));
        /*
        auto timePoint = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(timePoint);

        std::cout << "currentTime: " << currentTime << " isRunning: " << isRunning << std::endl;
        */
       //std::cout << "Send: " << sentBytes << "/" << fileSize << std::endl;
       status = sentBytes * 100 / fileSize + 1;
       
       std::cout << str;
       std::cout << " [" << std::right << std::setw(3) << status << "%]";
       std::cout << "\r";
    }
}

Timer::Timer(double timeoutMillis)
{
    this->timeout = timeoutMillis;
    this->isRunning = false;
}

Timer::~Timer()
{
    if(isRunning)
    {
        isRunning = false;
    }
}

// 装载定时触发器，定时触发进度条更新函数
void Timer::start(const size_t&fileSize, const size_t& sentBytes, const std::string str)
{
    isRunning = true;
    std::thread th = std::thread(loader, std::ref(isRunning), timeout, std::ref(fileSize), std::ref(sentBytes), std::ref(str));
    th.detach();
}

void Timer::stop()
{
    isRunning = false;
}
