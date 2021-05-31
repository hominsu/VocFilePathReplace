//
// Created by HominSu on 2021/5/31.
//

#ifndef VOCFILEPATHREPLACE__REPLACE_TASK_H_
#define VOCFILEPATHREPLACE__REPLACE_TASK_H_

#include <iostream>

#include "x_thread_pool.h"
#include "replace.h"

class ReplaceTask : public XTask {
 public:
  bool Run() override;

 public:
  std::vector<std::string> files_;
  std::string replace_str_;
};

#endif //VOCFILEPATHREPLACE__REPLACE_TASK_H_
