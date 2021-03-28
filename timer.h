#ifndef _TIMER_H
#define _TIMER_H

#include <iostream>
#include <iomanip>
#include <string>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <mutex>

extern std::mutex sleepMutex;
extern std::condition_variable condition;

class Timer
{
private:
    int timeout;
    bool isRunning;

public:;
    void start(const size_t&fileSize, const size_t& sentBytes, const std::string str);   
    void stop();

public:
    Timer(double timeout);
    ~Timer();
};

#endif
