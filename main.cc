#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "getfiles.h"
#include "x_thread_pool.h"
#include "replace_task.h"

void help(const char *_name) {
  std::string exec_file_name = _name;
  size_t lastPos = exec_file_name.find_last_of("/\\");
  exec_file_name = exec_file_name.substr(lastPos + 1);

  // 显示帮助
  std::cout << std::endl;
  std::cout << "This tool replaces the file path of the image in the XML file in the dataset" << std::endl << std::endl;
  std::cout << "Usage: " << std::endl;
  std::cout << "\t.\\" << exec_file_name << " [DataSet Path] [Replace Path]" << std::endl << std::endl;
  std::cout << "For example: " << std::endl;
  std::cout << "\t.\\" << exec_file_name << R"( C:\Users\17740\Desktop\DataSet\ D:\xyolo\images\train\)"
            << std::endl << std::endl;
}

int main(int argc, char **argv) {
  // 显示帮助
  if (argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
    help(argv[0]);
    return 0;
  }

  // 开始计时
  auto start_time_point = std::chrono::system_clock::now();

  std::string filePath = argv[1];
  std::string replace = argv[2];
  std::vector<std::string> files;

  // 规范化字符串
  if (filePath.find_last_of('\\') == filePath.length() - 1) {
    filePath.pop_back();
  }

  // 获取目录中的全部xml文件的路径
  getFiles(filePath, files);

  size_t size = files.size();

  // 获取线程数目
  unsigned int use_thread_num = std::thread::hardware_concurrency() - 1;
  if (1 == use_thread_num) {
    use_thread_num = 1;
  }
  std::cout << "Using " << use_thread_num << " Thread To Process Task" << std::endl;

  // 初始化线程池
  XThreadPool thread_pool;
  thread_pool.InitialThreadPool(use_thread_num);
  thread_pool.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // 任务队列
  std::vector<std::shared_ptr<ReplaceTask>> tasks;
  // 文件切分容器
  std::vector<std::vector<std::string>> task_files;

  // 文件替换任务切分
  for (unsigned int i = 0; i < use_thread_num; ++i) {
    unsigned int start, end;
    if (use_thread_num - 1 != i) {
      start = (size / use_thread_num) * i;
      end = (size / use_thread_num) * (i + 1) - 1;
      task_files.emplace_back(files.begin() + start, files.begin() + end);
    } else {
      start = (size / use_thread_num) * i;
      end = size - 1;
      task_files.emplace_back(files.begin() + start, files.begin() + end);
    }
  }

  // 添加任务到任务队列
  for (int i = 0; i < use_thread_num; ++i) {
    auto task = std::make_shared<ReplaceTask>();
    tasks.push_back(task);
    task->replace_str_ = replace;
    task->files_ = task_files[i];
    thread_pool.AddTask(task);
  }

  // 获取任务执行结果
  for (int i = 0; i < use_thread_num; ++i) {
    auto ret = tasks[i]->GetReturn();
    if (true == ret) {
      std::cout << "batch-" << i + 1 << ": succeed" << std::endl;
    } else {
      std::cout << "batch-" << i + 1 << ": failed" << std::endl;
    }
  }

  std::cout << "\rDone...          " << std::endl << std::endl;

  // 结束计时
  std::chrono::duration<double> diff = std::chrono::system_clock::now() - start_time_point;
  std::cout << "\tUsed: " << diff.count() << " Second" << std::endl;

  return 0;
}

// g++ --static main.cpp replace_.cpp -o main
