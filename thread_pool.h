//
// Created by zhazha on 11/15/25.
//

#ifndef DIR_MAPPER_THREAD_POOL_H
#define DIR_MAPPER_THREAD_POOL_H
#include <future>
#include <queue>

namespace execution {
    template <typename R>
    class thread_pool {
        int const max_threads;
        int current_threads;
        bool should_shutdown;
        std::queue<std::thread> workers;
        std::condition_variable work_cv;
        std::condition_variable worker_queue_cv;
        std::mutex parallel_size_mutex;
        std::mutex worker_queue_mutex;
        std::thread collect_thread;
        void join();
        void start();
        void shutdown();

    public:
        explicit thread_pool(int const max_threads):
        max_threads{max_threads}, current_threads{0}, should_shutdown{false} {
            this->start();
        }
        ~thread_pool() {
            this->shutdown();
        }
        std::future<R> submit(std::function<R()> work);
    };

    template<typename R>
    std::future<R> thread_pool<R>::submit(std::function<R()> work) {
        std::promise<R> work_promise;
        std::future<R> work_future = work_promise.get_future();
        {
            std::unique_lock enqueue_lock{parallel_size_mutex};
            work_cv.wait(enqueue_lock, [this] {
                return current_threads < max_threads;
            });

            {
                std::lock_guard worker_lock{worker_queue_mutex};
                if (this->should_shutdown)
                    throw std::runtime_error("It shouldn't be possible te submit task after thread pool shutdown");
                workers.emplace([this, cap_work = std::move(work), promise = std::move(work_promise)]() mutable  {
                    promise.set_value(cap_work());

                    {
                        std::lock_guard lock_finish{parallel_size_mutex};
                        --current_threads;
                    }
                    work_cv.notify_one();
                });
            }
            worker_queue_cv.notify_one();

            ++current_threads;
        }

        return work_future;
    }

    template<typename R>
    void thread_pool<R>::start() {
        collect_thread = std::thread([this]{this->join();});
    }

    template<typename R>
    void thread_pool<R>::join() {
        bool found_shutdown = false;
        while (!found_shutdown) {
            std::vector<std::thread> current_batch;
            {
                std::unique_lock lock{worker_queue_mutex};
                worker_queue_cv.wait(lock, [this] {
                   return this->should_shutdown || !this->workers.empty();
                });
                found_shutdown = this->should_shutdown;
                while (!this->workers.empty()) {
                    current_batch.emplace_back(std::move(this->workers.front()));
                    this->workers.pop();
                }
            }
            for (auto & thread: current_batch) {
                thread.join();
            }
        }
    }

    template<typename R>
    void thread_pool<R>::shutdown() {
        {
            std::lock_guard lock{worker_queue_mutex};
            this->should_shutdown = true;
        }
        worker_queue_cv.notify_one();
        collect_thread.join();
    }
} // execution

#endif //DIR_MAPPER_THREAD_POOL_H