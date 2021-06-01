//
// Created by HominSu on 2021/5/21.
//

#include "x_thread_pool.h"

XThreadPool::XThreadPool() = default;

XThreadPool::~XThreadPool() {
  if (false == is_exit()) {
    Stop();
  }
}

/**
 * @brief 初始化线程池
 * @param _thread_num 线程数量
 */
void XThreadPool::InitialThreadPool(const unsigned int &_thread_num) {
  std::unique_lock<std::mutex> lock(mutex_);
  this->thread_num_ = _thread_num;
  this->threads_.reserve(_thread_num);
}

/**
 * @brief 启动所有线程，必须先初始化
 */
void XThreadPool::Start() {
  std::unique_lock<std::mutex> lock(mutex_);

  if (this->thread_num_ <= 0) {
    std::cerr << "ThreadPool need initial first" << std::endl;
  }

  if (!this->threads_.empty()) {
    std::cerr << "ThreadPool is started" << std::endl;
  }

  for (int i = 0; i < this->thread_num_; i++) {
    //auto th = new std::thread(&XThreadPool::Run, this);
    auto th = std::make_shared<std::thread>(&XThreadPool::Run, this);
    this->threads_.push_back(th);
  }
}

void XThreadPool::Stop() {
  this->is_exit_ = true;
  this->cv_.notify_all();

  // 等待线程退出
  for (auto &th : this->threads_) {
    try {
      th->join();
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    } catch (...) {
      std::cout << "wrong" << std::endl;
    }
  }

  // 清理线程池
  std::unique_lock<std::mutex> lock(this->mutex_);
  this->threads_.clear();
}

void XThreadPool::AddTask(std::shared_ptr<XTask> _task) {
  std::unique_lock<std::mutex> lock(this->mutex_);
  _task->is_exit = [this] { return is_exit(); };
  this->tasks_.push_back(_task);

  lock.unlock();
  this->cv_.notify_one();
}

std::shared_ptr<XTask> XThreadPool::GetTask() {
  std::unique_lock<std::mutex> lock(this->mutex_);
  if (this->tasks_.empty()) {
    this->cv_.wait(lock);
  }

  // 线程池为退出状态时，返回空指针
  if (true == is_exit()) {
    return nullptr;
  }

  // 防止多次通知
  if (this->tasks_.empty()) {
    return nullptr;
  }

  auto task = this->tasks_.front();
  this->tasks_.pop_front();

  return task;
}

void XThreadPool::Run() {
  std::cout << "Run " << std::this_thread::get_id() << std::endl;
  while (false == is_exit()) {
    auto task = GetTask();

    // 获得的任务指针为空，继续等待获取
    if (!task) {
      continue;
    }

    ++this->task_run_count_;  // 增加当前运行的任务数量

    // 执行线程任务
    try {
      auto ret = task->Run();
      task->SetValue(ret);
    } catch (...) {
      std::cerr << "task run failed" << std::endl;
    }

    --this->task_run_count_;  // 减去当前运行的任务数量
  }
}
