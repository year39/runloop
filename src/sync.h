
#pragma once
#include <fcntl.h>
#include <unistd.h>

namespace runloop::details {
class Sync {
 public:
  Sync() {
    pipe(fds);
  }

  ~Sync() {
    wakeup();
    while (waiting == true) {
      usleep(1000);
      wakeup();
    }
  }

  void wait() {
    char c;
    waiting = true;
    read(fds[0], &c, 1);
    waiting = false;
  }

  void wakeup() {
    write(fds[1], "*", 1);
  }

 private:
  int fds[2] {0};
  bool waiting {false};
};
}  // namespace runloop::details
