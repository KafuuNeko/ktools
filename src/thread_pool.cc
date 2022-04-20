#include "thread_pool.hpp"
#include "def_export.hpp"
#include <stdint.h>


typedef void (* __stdcall CallbackT)(void*);

struct ThreadPoolManager {
    ThreadPool threadpool;
    CallbackT callback;
};

using ThreadPoolManagerPtr = ThreadPoolManager*;

EXPORT_API(ThreadPoolManagerPtr, new_thread_pool, (parameterSizeof<uint32_t, CallbackT>()))
(uint32_t size, CallbackT callback)
{
    return new ThreadPoolManager { ThreadPool { size }, callback };
}

EXPORT_API(void, free_thread_pool, (parameterSizeof<thread_pool_manager*>()))
(ThreadPoolManagerPtr manager)
{
    if (manager) {
        delete manager;
    }
}

EXPORT_API(void, thread_commit, (parameterSizeof<thread_pool_manager*, void*>()))
(ThreadPoolManagerPtr manager, void* contents)
{
    if (manager) {
        auto callback = manager->callback;
        manager->threadpool.commit([callback, contents](){
            callback(contents);
        });
    }
}

EXPORT_API(int32_t, thread_idle, (parameterSizeof<thread_pool_manager*>()))
(ThreadPoolManagerPtr manager)
{
    return manager ? manager->threadpool.idleThread() : 0;
}

EXPORT_API(void, thread_add, (parameterSizeof<thread_pool_manager*, int32_t>()))
(ThreadPoolManagerPtr manager, int32_t size)
{
    if (manager) {
        manager->threadpool.addThread(size);
    }
}
