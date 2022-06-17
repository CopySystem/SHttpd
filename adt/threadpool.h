#include <atomic>
#include <thread>
#include <vector>
#include <functional>

#include "threadsafequeue.h"

#ifndef THREAD_POOL_LYOZG
#define THREAD_POOL_LYOZG

class join_threads {
    std::vector<std::thread>& threads;
    public:
        explicit join_threads(std::vector<std::thread>& threads_): threads(threads_) {}
        ~join_threads() {
            for(unsigned long i=0;i<threads.size();++i) {
                if(threads[i].joinable())
                threads[i].join();
            }
        }
};


class thread_pool {
    private :
        std::atomic_bool done ;
        threadsafe_queue<std::function<void()> > work_queue;
        std::vector<std::thread> threads;
        join_threads joiner;

        void worker_thread() ;

    public :
        thread_pool() ;
        ~thread_pool() ;
        template<typename FunctionType> void submit(FunctionType f) ;
} ;

#endif