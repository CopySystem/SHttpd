#include <queue>
#include <mutex>
#include <thread>
#include <memory>
#include <condition_variable>

#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

template <typename T>
class threadsafe_queue {
    private :
        mutable std::mutex mut ;
        std::queue<T> data_queue ;
        std::condition_variable data_cond ;

    public :
        threadsafe_queue() = default ;
        void push(T new_value) ;
        void wait_and_pop(T &value) ;
        std::shared_ptr<T> wait_and_pop() ;
        bool try_pop(T &value) ;
        std::shared_ptr<T> try_pop() ;
        bool empty() const ;
} ;

#endif