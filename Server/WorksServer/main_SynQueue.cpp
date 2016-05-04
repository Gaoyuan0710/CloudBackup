#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <chrono>
#include "ThreadPool.h"

void func()
{
    std::cout << "我要执行啦" << std::endl;
}

using task = std::function<void()>;

int main(int argc, char *argv[])
{
    ThreadPool threadpool(10);
    std::thread thread([&threadpool]
            {
                for(int i = 0; i < 10; i++)
                {
                    threadpool.AddTask([]()
                        {
                            int sum = 0;
                            std::cout << "ID:" << std::this_thread::get_id() << " 执行任务" << std::endl;
                            for(int i = 0; i < 1000000; i++)
                            {
                                sum += i;
                            }
                        });
                }
            });

    std::this_thread::sleep_for(std::chrono::seconds(2));
    threadpool.Stop();
    thread.join();

	return EXIT_SUCCESS;
}


