#include<iostream>
#include<string>
#include<cstdlib>
#include<cerrno>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<thread>
#include<list>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<atomic>
#include<stdio.h>

const int MaxTaskCountk = 1000;
template<typename T>
class SyncQueue{
    public:
        SyncQueue(int max_size_) : max_size_(max_size_) , stop_flag_(false) {}
        ~SyncQueue(){}

        void Put(const T &x ) {
            Add(x);
        }

        void Put( T && x)  {
            Add(std::forward<T>(x));
        }

        void Take(std::list<T> & list) {
            std::unique_lock<std::mutex> locker(mutex_);
            not_empty_condition_.wait( locker , [this]{return stop_flag_ || NotEmpty();});
            if( stop_flag_ ) {
                return;
            }
            list = queue_list_;
            for(int i = 0 ; i < list.size() ; i++)  {
                queue_list_.pop_back();
            }
            not_full_condition_.notify_one();
        }

        void Take( T & t ) {
            std::unique_lock<std::mutex> locker(mutex_);
            not_empty_condition_.wait( locker , [this]{ return stop_flag_ || NotEmpty();});
            if(stop_flag_)  {
                return;
            }
            t = queue_list_.front();
            queue_list_.pop_front();
            not_full_condition_.notify_one();
        }

        void Stop()  {
            {
                std::lock_guard<std::mutex> locker(mutex_);
                stop_flag_ = true;
            }
            not_full_condition_.notify_all();
            not_empty_condition_.notify_all();
        }

        bool Empty(){
            std::lock_guard<std::mutex> locker(mutex_);
            return queue_list_.size();
        }

        bool Full()  {
            std::lock_guard<std::mutex> locker(mutex_);
            return queue_list_.size() == max_size_;
        }

        size_t Size() {
            std::lock_guard<std::mutex> locker(mutex_);
            return queue_list_.size();
        }

        int Count()  {
            return queue_list_.size();
        }

    private:
        bool NotFull()  {
            bool full = queue_list_.size() >= max_size_;
            if( full )  {
                std::cout << " The Syncqueue is full ..." << std::endl;
            }
            return !full;
        }

        bool NotEmpty()  {
            bool empty = queue_list_.empty();
            if( empty )  {
                std::cout << "The Syncqueue is Empty..." << std::endl;
            }
            return !empty;
        }

        template<typename F>
        void Add( F && x ) {
            std::unique_lock<std::mutex> locker(mutex_);
            not_full_condition_.wait(locker,[this]{return stop_flag_ || NotFull();});
            if( stop_flag_ )  {
                return;
            }
            queue_list_.push_back(std::forward<F>(x));
            not_empty_condition_.notify_one();
        }
    public:
        std::list<T> queue_list_;
        std::mutex mutex_;
        std::condition_variable not_empty_condition_;
        std::condition_variable not_full_condition_;
        int max_size_;
        bool stop_flag_;
};

template<typename T>
class ThreadPool{
    public:
//        using Task = std::function<void()>;
        ThreadPool(int numThreads = std::thread::hardware_concurrency()):syncque_(MaxTaskCountk)  {
            Start(numThreads);
        }

        ~ThreadPool(void) {
            StopThreadGroup();
        }


        void AddTask(T & t) {
            syncque_.Put(t);
        }

        void Start(int numThreads)  {
            running_flag_ = true;
            for(int i = 0 ; i < numThreads ; ++i ) {
                thread_group_ptr_.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread , this));
            }
        }

        void RunInThread() {
            while(running_flag_)  {
                std::list<T> list;
                syncque_.Take(list);
                for( auto & k : list )  {
                    if(!running_flag_)  {
                        return;
                    }
                    k.UserRequest();
                }
            }
        }

        void StopThreadGroup()  {
            syncque_.Stop();
            running_flag_ = false;
            for( auto thread : thread_group_ptr_ )  {
                if(thread)  {
                    thread->join();
                }
                thread_group_ptr_.clear();
            }
        }

        void Stop()
        {
            std::call_once(once_flag_,[this]{StopThreadGroup();});
        }

        std::list<std::shared_ptr<std::thread>> thread_group_ptr_;
        SyncQueue<T> syncque_;
        std::atomic_bool running_flag_;
        std::once_flag once_flag_;
};
