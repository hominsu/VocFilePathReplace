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
  std::cout << "\t.\\" << exec_file_name << " C:\\Users\\17740\\Desktop\\DataSet\\ D:\\xyolo\\images\\train\\"
            << std::endl << std::endl;
}

int main(int argc, char **argv) {
  // 显示帮助
  if (argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
    help(argv[0]);
    return 0;
  }

  // 开始计时
  auto start = std::chrono::system_clock::now();

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

  // 初始化线程池
  unsigned int core_num = std::thread::hardware_concurrency();

  XThreadPool thread_pool;
  thread_pool.InitialThreadPool(core_num - 1);
  thread_pool.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // 添加任务到任务队列
  std::vector<std::shared_ptr<ReplaceTask>> tasks;

  for (int i = 0; i < core_num - 1; ++i) {
    auto task = std::make_shared<ReplaceTask>();
    tasks.push_back(task);
    task->replace_str_ = replace;
    task->files_ = files;
    thread_pool.AddTask(task);
  }

  for (int i = 0; i < core_num - 1; ++i) {
    auto ret = tasks[i]->GetReturn();
    std::cout << "task" << i << ": " << ret << std::endl;
  }

  std::cout << "\rDone...          " << std::endl << std::endl;

  // 结束计时
  std::chrono::duration<double> diff = std::chrono::system_clock::now() - start;
  std::cout << "\tUsed: " << diff.count() << " Second" << std::endl;

  return 0;
}

// g++ --static main.cpp replace_.cpp -o main
