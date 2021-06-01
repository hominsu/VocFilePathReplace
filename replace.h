#include <cstring>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const char kFileNameLabelStart[] = "<filename>";
const char kFileNameLabelEnd[] = "</filename>";
const char kPathLabelStart[] = "<path>";
const char kPathLabelEnd[] = "</path>";

class Replace {
 private:
  std::string str_;
  std::fstream fp_;
  char buf[1024]{};

  std::vector<std::string> files_;
  std::string replace_str_;

 private:
  const std::string filename_label_start_ = kFileNameLabelStart;
  const std::string filename_label_end_ = kFileNameLabelEnd;
  const std::string path_label_start_ = kPathLabelStart;
  const std::string path_label_end_ = kPathLabelEnd;

 private:
  std::string replace_;
  std::string xml_name_;
  std::string filename_;
  size_t filename_pos_start_ = 0;
  size_t filename_pos_end_ = 0;
  size_t filename_length_ = 0;
  size_t path_pos_start_ = 0;
  size_t path_pos_end_ = 0;
  size_t path_length_ = 0;

 public:
  explicit Replace(std::string  _replace_str, std::vector<std::string> _files);

  void Do();

 private:
  void Initial(const std::string &_replace, const std::string &_xml_name);
  bool ReadFile();
  void ReplaceStr();

};
