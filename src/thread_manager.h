#pragma once

#include "sync.h"
#include <functional>
#include <thread>

namespace runloop::details {

class ThreadManger: private Sync {
 public:
  ThreadManger(std::function<void()> worker) {
    process = worker;
    t       = std::thread {std::bind(&ThreadManger::mainloop, this)};
  }

  ~ThreadManger() {
    terminate();
    join();
  }

  void notify() {
    wakeup();
  }

  void terminate() {
    terminateReq = true;
    notify();
  }

  bool terminating() {
    return terminateReq;
  }

  void join() {
    if (joinable()) t.join();
  }

  bool joinable() {
    return t.joinable();
  }

 private:
  void mainloop() {
    while (!terminating()) {
      process();
      wait();
    }
  }

 private:
  std::function<void()> process;
  std::thread t;
  bool terminateReq {false};
};

}  // namespace runloop::details
