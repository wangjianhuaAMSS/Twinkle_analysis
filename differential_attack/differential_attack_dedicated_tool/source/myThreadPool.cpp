#include "../include/myThreadPool.h"

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (auto &worker : workers)
        worker.join();
}

ThreadPool::ThreadPool(size_t nr_threads) : stop(false)
{
    for (size_t i = 0; i < nr_threads; ++i)
    {
        std::thread worker([this]() {
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                    if (stop && tasks.empty())
                        return;

                    task = std::move(tasks.front());
                    ++taskCount;
                    tasks.pop();
                }
                task();
                std::unique_lock<std::mutex> lock(mtx);
                --taskCount;
            }
        });
        workers.emplace_back(std::move(worker));
    }
}

template <class F, class... Args>
std::future<std::result_of_t<F(Args...)>> ThreadPool::enqueue(F &&f, Args &&...args)
{
    using return_type = std::result_of_t<F(Args...)>;
    
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mtx);

        if (stop)
            throw std::runtime_error("The thread pool has been stop.");
        
        tasks.emplace([task]() -> void { (*task)(); });
    }
    cv.notify_one();
    return res;
}
template std::future<std::result_of<int (&(TrailSeedIterator&))(TrailSeedIterator const&)>::type> ThreadPool::enqueue<int (&)(TrailSeedIterator const&), TrailSeedIterator&>(int (&)(TrailSeedIterator const&), TrailSeedIterator&);



bool ThreadPool::isstop() {
    std::unique_lock<std::mutex> lock(mtx);
    return((taskCount == 0) && tasks.empty()); 
}

