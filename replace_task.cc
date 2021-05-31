//
// Created by HominSu on 2021/5/31.
//

#include "replace_task.h"

bool ReplaceTask::Run() {
  auto replace_ = std::make_shared<Replace>(this->replace_str_, this->files_);
  replace_->Do();
  return true;
}