//
// Created by HominSu on 2021/5/21.
//

#ifndef GESTURERECOGNITIONSERVER__X_THREAD_POOL_H_
#define GESTURERECOGNITIONSERVER__X_THREAD_POOL_H_

#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <future>

/**
 * @brief 线程任务
 */
class XTask {
 public:
  virtual bool Run() = 0;
  std::function<bool()> is_exit = nullptr;
  auto GetReturn() {
    // 阻塞等待返回值
    return this->p_.get_future().get();
  }
  void SetValue(int _value) {
    this->p_.set_value(_value);
  }

 private:
  std::promise<int> p_;
};

/**
 * @brief 线程池基类
 */
class XThreadPool {
 private:
  unsigned int thread_num_ = 0;  ///< 线程数量
  std::vector<std::shared_ptr<std::thread>> threads_;  ///< 用于存放线程句柄
  std::mutex mutex_;  ///< 互斥量
  std::condition_variable cv_;  ///< 信号量
  std::list<std::shared_ptr<XTask>> tasks_;  ///< 线程任务列表
  bool is_exit_ = false; ///< 线程池的退出，该变量不用考虑线程安全
  std::atomic<int> task_run_count_{0};    ///< 正在运行的任务数量，原子变量，线程安全

 public:
  XThreadPool();
  ~XThreadPool();

  void InitialThreadPool(const unsigned int &_thread_num);
  void Start();
  void Stop();
  void AddTask(std::shared_ptr<XTask> _task);
  std::shared_ptr<XTask> GetTask();

  [[nodiscard]] bool is_exit() const { return this->is_exit_; }
  int task_run_count() { return this->task_run_count_; }

 private:
  void Run();
};

#endif //GESTURERECOGNITIONSERVER__X_THREAD_POOL_H_
