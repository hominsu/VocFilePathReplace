#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "replace.h"
#include "getfiles.h"
#include "x_thread_pool.h"
#include "replace_task.h"

void help(const char *name) {
  std::string progname = name;
  size_t lastPos = progname.find_last_of("/\\");
  progname = progname.substr(lastPos + 1);

  // 显示帮助
  std::cout << std::endl;
  std::cout << "This tool replaces the file path of the image in the XML file in the dataset" << std::endl << std::endl;
  std::cout << "Usage: " << std::endl;
  std::cout << "\t.\\" << progname << " [DataSet Path] [Replace Path]" << std::endl << std::endl;
  std::cout << "For example: " << std::endl;
  std::cout << "\t.\\main.exe C:\\Users\\17740\\Desktop\\DataSet\\ D:\\xyolo\\images\\train\\"
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

  size_t i = 0;
  size_t size = files.size();

  // 初始化线程池
  XThreadPool thread_pool;
  thread_pool.InitialThreadPool(1);
  thread_pool.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // 添加任务
  auto task1 = std::make_shared<ReplaceTask>();
  task1->replace_str_ = replace;
  task1->files_ = files;
  thread_pool.AddTask(task1);

  auto ret_1 = task1->GetReturn();
  std::cout << "task1: " << ret_1 << std::endl;


  std::cout << "\rDone...          " << std::endl << std::endl;

  // 结束计时
  std::chrono::duration<double> diff = std::chrono::system_clock::now() - start;
  std::cout << "\tUsed: " << diff.count() << " Second" << std::endl;

  return 0;
}

// g++ --static main.cpp replace_.cpp -o main
