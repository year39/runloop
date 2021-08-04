#include "runloop/loop_imp.h"
#include "thread_manager.h"
#include <csignal>

using namespace runloop::details;

void signal_handler(int signal) {
  if (signal == SIGINT) LoopImp::instance().terminate();
}

LoopImp::LoopImp() {
  manager = std::make_unique<ThreadManger>(std::bind(&LoopImp::process, this));
  std::signal(SIGINT, signal_handler);
}

LoopImp::~LoopImp() {
  manager->terminate();
}

void LoopImp::process() {
  std::queue<Task> queue_local;
  {
    std::lock_guard<std::mutex> lock(mutex);
    queue_local = std::move(queue);
  }
  while (!queue_local.empty()) {
    queue_local.front()();
    queue_local.pop();
  }
}

bool LoopImp::idle() {
  return queue.size() == 0;
}

void LoopImp::invoke(Task task) {
  std::lock_guard<std::mutex> lock(mutex);
  queue.push(std::move(task));
  manager->notify();
}

void LoopImp::join() {
  std::lock_guard<std::mutex> lock(joinmtx);
  manager->join();
}

bool LoopImp::joinable() {
  return manager->joinable();
}

void LoopImp::terminate() {
  manager->terminate();
}
