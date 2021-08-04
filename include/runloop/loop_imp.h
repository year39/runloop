#pragma once
#include <functional>
#include <memory>
#include <mutex>
#include <queue>

namespace runloop {
  using Task = std::function<void()>;

  namespace details {

    class ThreadManger;

    class LoopImp {
     public:
      LoopImp();
      LoopImp(const LoopImp&) = delete;
      LoopImp& operator=(const LoopImp&) = delete;
      ~LoopImp();
      void join();
      bool joinable();
      void terminate();
      bool idle();
      void invoke(Task task);
      static LoopImp& instance() {
        static LoopImp imp;
        return imp;
      }

     private:
      std::unique_ptr<ThreadManger> manager;

      void process();

      std::mutex mutex;
      std::mutex joinmtx;
      std::queue<Task> queue;
    };

  }  // namespace details
}  // namespace runloop
