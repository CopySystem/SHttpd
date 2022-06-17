#include <atomic>
#include <thread>
#include <vector>
#include <functional>

#include <iostream>

#include "threadpool.h"
#include "threadsafequeue.h"

void thread_pool::worker_thread() {
    std::function<void()> task ;
    while (!done) {
        if ( work_queue.try_pop( task ) ) 
            task() ;
        else std::this_thread::yield() ;
    }
}

thread_pool::thread_pool():
    done(false),joiner(threads)
{
    unsigned const thread_count = static_cast<int>(std::thread::hardware_concurrency() * 1.5) ; 
    try {
        for(unsigned i=0;i<thread_count;++i) {
            threads.push_back( std::thread(&thread_pool::worker_thread,this) );
        }
    } catch(...)  {
        done=true; 
        throw;
    }
}

thread_pool::~thread_pool() {
    done = true ;
}

template void thread_pool::submit(std::function<void()> f) ;

template <typename FunctionType> 
void thread_pool::submit(FunctionType f) {
    work_queue.push(std::function<void()>(f)) ;
}