#pragma once

/**
 * @file thread_pool.hpp
 * @author kafuu (kafuuneko@gmail.com)
 * @brief 线程池
 * @version 0.1
 * @date 2022-04-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <condition_variable>

#include <cinttypes>

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(size_t size) noexcept
        : mSize(size), isRunning(true), mIdleThread(0)
    {
        addThread(size);
    }

    ~ThreadPool() noexcept {
        this->isRunning = false;
        mCv.notify_all();

        for (auto &pool : mPool) {
            if (pool.joinable()) {
                pool.join();
            }
        }
    }

    void addThread(size_t size) noexcept
    {
        if (size == 0) {
            return;
        }

        mPool.emplace_back([=]() {
            while (isRunning) {
                waitAndProcess();
            }
        });

        ++mSize;
        this->addThread(size - 1);
    }

    size_t idleThread() const {
        return static_cast<size_t>(mIdleThread);
    }

    template<typename... Args>
    void commit(Args&&... args) {
        mTasks.emplace(std::forward<Args>(args)...);
        mCv.notify_one();
    }

private:
    size_t mSize;
    std::vector<std::thread> mPool;
    std::queue<Task> mTasks;

    std::mutex mMutex;
    std::condition_variable mCv;

    std::atomic_size_t mIdleThread;

    bool isRunning;

    void waitAndProcess()
    {
        std::unique_lock<std::mutex> lk(mMutex);
        
        mIdleThread++;
        mCv.wait(lk, [this] {
            return !mTasks.empty() || !isRunning;
        });
        mIdleThread--;

        if (!isRunning) {
            return;
        }

        auto task = mTasks.front();
        mTasks.pop();

        lk.unlock();

        //procee
        task();
    }
};
