#pragma once
#include "runloop/loop_imp.h"

namespace runloop {
  inline void init() {
    details::LoopImp::instance();
  }

  inline void join() {
    details::LoopImp::instance().join();
  }

  inline bool joinable() {
    return details::LoopImp::instance().joinable();
  }

  inline void terminate() {
    details::LoopImp::instance().terminate();
  }

  inline bool idle() {
    return details::LoopImp::instance().idle();
  }

  inline void invoke(Task task) {
    details::LoopImp::instance().invoke(task);
  }
}  // namespace runloop
