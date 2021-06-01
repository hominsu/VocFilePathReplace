//
// Created by HominSu on 2021/5/31.
//

#include "replace_task.h"

bool ReplaceTask::Run() {
  this->Do(this->replace_str_, this->files_);
  return true;
}

void ReplaceTask::Initial(const std::string &_replace, const std::string &_xml_name) {
  this->replace_ = _replace;
  this->xml_name_ = _xml_name;

  if (this->replace_.find_last_of('\\') < this->replace_.length() - 1) {
    this->replace_.push_back('\\');
  }
}

void ReplaceTask::ReadFile() {
  this->fp_.open(xml_name_.c_str());

  if (!fp_.is_open()) {
    throw std::runtime_error(xml_name_ + std::string(" open failed"));
  }

  while (!fp_.eof()) {
    memset(this->buf, '\0', sizeof(this->buf));
    this->fp_.getline(this->buf, 1024);
    this->str_.append(this->buf);
    this->str_.append("\n");
  }
  this->str_.pop_back();
}

void ReplaceTask::ReplaceStr() {
  // 从 filename 标签中截取 filename
  this->filename_pos_start_ = this->str_.find(this->filename_label_start_);
  this->filename_pos_end_ = this->str_.find(this->filename_label_end_);
  this->filename_length_ = this->filename_pos_end_ - this->filename_pos_start_ - this->filename_label_start_.length();
  this->filename_ =
      str_.substr(this->filename_pos_start_ + this->filename_label_start_.length(), this->filename_length_);

  // 将 path 标签中的文件路径替换为定义的路径加上文件名
  this->path_pos_start_ = this->str_.find(this->path_label_start_);
  this->path_pos_end_ = this->str_.find(this->path_label_end_);
  this->path_length_ = this->path_pos_end_ - this->path_pos_start_ - this->path_label_start_.length();
  this->str_.replace(this->path_pos_start_ + this->path_label_start_.length(),
                     this->path_length_,
                     (this->replace_ + this->filename_).c_str());

  this->fp_.clear();
  this->fp_.seekp(0, std::ios::beg);
  this->fp_ << str_;
  this->fp_.close();
}

void ReplaceTask::Do(std::string _replace_str, std::vector<std::string> _files) {
  this->replace_str_ = std::move(_replace_str);
  this->files_ = std::move(_files);

  while (!this->files_.empty()) {
    auto xml_file_name = this->files_.back();
    this->files_.pop_back();

    try {
      Initial(this->replace_str_, xml_file_name);
      ReadFile();
      ReplaceStr();
    } catch (std::runtime_error &r) {
      std::cerr << r.what() << std::endl;
    } catch (...) {
      std::cerr << "Unknown Wrong" << std::endl;
    }

  }
}
