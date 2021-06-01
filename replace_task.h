//
// Created by HominSu on 2021/5/31.
//

#ifndef VOCFILEPATHREPLACE__REPLACE_TASK_H_
#define VOCFILEPATHREPLACE__REPLACE_TASK_H_

#include <cstring>

#include <iostream>
#include <fstream>

#include "x_thread_pool.h"

const char kFileNameLabelStart[] = "<filename>";
const char kFileNameLabelEnd[] = "</filename>";
const char kPathLabelStart[] = "<path>";
const char kPathLabelEnd[] = "</path>";

class ReplaceTask : public XTask {
 public:
  bool Run() override;

 public:
  std::vector<std::string> files_;  ///< 保存替换的文件
  std::string replace_str_; ///< 替换的地址

 private:
  std::string str_; ///< 保存从文件读取的字符
  std::fstream fp_; ///< 文件流
  char buf[1024]{}; ///< 文件字符读取缓存

 private:
  const std::string filename_label_start_ = kFileNameLabelStart;  ///< 文件名称开始标识符
  const std::string filename_label_end_ = kFileNameLabelEnd;  ///< 文件名称结束标识符
  const std::string path_label_start_ = kPathLabelStart;  ///< 路径开始标识符
  const std::string path_label_end_ = kPathLabelEnd;  ///< 路径结束标识符

 private:
  std::string replace_; ///< 替换的地址的中间变量
  std::string xml_name_;  ///< 替换的 xml 文件对象路径
  std::string filename_;  ///< 替换的 xml 文件对象名称
  size_t filename_pos_start_ = 0;
  size_t filename_pos_end_ = 0;
  size_t filename_length_ = 0;
  size_t path_pos_start_ = 0;
  size_t path_pos_end_ = 0;
  size_t path_length_ = 0;

 public:
  void Do(std::string _replace_str, std::vector<std::string> _files);

 private:
  void Initial(const std::string &_replace, const std::string &_xml_name);
  void ReadFile();
  void ReplaceStr();
};

#endif //VOCFILEPATHREPLACE__REPLACE_TASK_H_
