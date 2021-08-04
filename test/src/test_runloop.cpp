#include <iostream>
#include <runloop/runloop.h>
#include <string>
#include <thread>
#include <unistd.h>

void callback1(std::string s) {
  std::cout << "callback1 " << s << "\n";
}

int main() {
  runloop::invoke([] { std::cout << "Hola!\n"; });
  runloop::invoke([] {
    sleep(1);
    callback1("Hola");
  });

  std::thread t0[100];
  for (int i = 0; i < 100; i++) {
    t0[i] = std::thread {[=]() {
      runloop::invoke([] {
        std::cout << "hello from thread"
                  << "\n";
      });
      runloop::join();
    }};
  }

  std::thread t3 {[] {
    runloop::invoke([] { std::cout << "hello from thread3\n"; });
    sleep(5);
    runloop::terminate();
    runloop::terminate();
    runloop::join();
  }};

  std::cout << "terminating...\n";
  sleep(2);
  if (runloop::joinable()) {
    runloop::terminate();
    std::cout << "joining...\n";
    runloop::join();
  }
  std::cout << "t1 join\n";
  for (int i = 0; i < 100; i++) {
    t0[i].join();
  }
  t3.join();
  return 0;
}
