#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_
#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <assert.h>
#include "myTrailSeedIterator.h"

class ThreadPool
{
  public:
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    ThreadPool(size_t nr_threads);
    virtual ~ThreadPool();

    template <class F, class... Args>
    std::future<std::result_of_t<F(Args...)>> enqueue(F &&f, Args &&...args);
    bool isstop();

  private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
    int taskCount = 0;
};
#endif
