#include "synchronization.h"

#include <Common.h>

#include <future>
#include <thread>
#include <vector>

#include "ThreadGuard.h"

namespace synchronization {
/*
在线程间传递数据
1.简单异步任务	std::async
2.可调用对象包装	std::packaged_task
3.完全手动控制	std::promise
*/
std::queue<int> dataQueue;
std::mutex mutex;
std::condition_variable cond;
int value;

void product() {
    while (value < 100) {
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << std::this_thread::get_id() << ",product : " << value
                      << "size:" << dataQueue.size() << std::endl;
            dataQueue.push(value++);
        }
        // 唤醒一个正在等待该条件变量的线程（具体唤醒哪个线程由系统调度决定）
        cond.notify_one();
        // std::cout << "product notify" << std::endl;
    }
}

void consume() {
    while (value < 100) {
        // std::cout << "Consume" << std::endl;
        // unique_lock可以灵活加锁解锁
        std::unique_lock<std::mutex> lock(mutex);
        // wait在条件满足时，直接返回，继续运行；条件不满足时，解锁互斥，阻塞线程；
        // 线程阻塞后又被唤醒时，wait重新给互斥加锁(所以必须用unique_lock)；
        cond.wait(lock, [] { return dataQueue.size(); });
        auto data = dataQueue.front();
        dataQueue.pop();
        lock.unlock();
        std::cout << std::this_thread::get_id() << ",Consume:" << data
                  << "size:" << dataQueue.size() << std::endl;
    }
}

void testConditionVariable() {
    PRINT_FUNC_NAME();
    std::thread productor(product);
    std::thread consumer(consume);
    productor.join();
    consumer.join();
    std::cout << "testConditionVariable END" << std::endl;
}

void product2(threadSafeQueue<int> &queue) {
    while (value < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        queue.push(value++);
        std::cout << std::this_thread::get_id() << ",product : " << value
                  << "size:" << dataQueue.size() << std::endl;
    }
}

void consume2(threadSafeQueue<int> &queue) {
    while (value < 100) {
        int data;
        queue.waitAndPop(data);
        std::cout << std::this_thread::get_id() << ",Consume:" << data
                  << "size:" << dataQueue.size() << std::endl;
    }
}

void consume3(threadSafeQueue<int> &queue) {
    while (value < 100) {
        auto dataPtr = queue.waitAndPop();
        std::cout << std::this_thread::get_id() << ",Consume:" << *dataPtr
                  << "size:" << dataQueue.size() << std::endl;
    }
}

void testConditionVariable2() {
    PRINT_FUNC_NAME();

    threadSafeQueue<int> queue;
    std::thread t1(product2, std::ref(queue));
    std::thread t2(consume2, std::ref(queue));
    std::thread t3(consume3, std::ref(queue));

    t1.join();
    t2.join();
    t3.join();
    std::cout << " testConditionVariable2 END" << std::endl;
}

void testasync() {
    auto getResult = []() -> int {
        std::cout << "aync id:" << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 100;
    };
    // std::async与std::thread 相比：1.不立即启动，根据调度策略可能延迟启动
    // 2.不需要join、detach，自动管理线程生命周期 3.异步任务结果通过future获取
    // 不需要创建共享变量 4.适合短期任务

    // 显示指定调度策略
    // 1.默认策略：async|deferred,自行选择执行方式
    // 2.async:强制创建新线程执行任务,不保证立即执行
    // 3.deferred:延迟到调用get()时运行,不创建新线程，在调用get的线程中运行
    std::cout << "id:" << std::this_thread::get_id() << std::endl;

    std::future<int> result = std::async(getResult);
    std::future<int> result_async = std::async(std::launch::async, getResult);
    std::future<int> result_defered =
        std::async(std::launch::deferred, getResult);
    // result.wait();
    std::cout << "testasync result get: " << result.get() << std::endl;
    std::cout << "testasync result_async get: " << result_async.get()
              << std::endl;
    std::cout << "testasync result_defered get: " << result_defered.get()
              << std::endl;
    std::cout << "testasync End" << std::endl;
}

void testPromise() {
    std::promise<int> prom;
    // future 与promise建立连接
    std::future<int> fut = prom.get_future();
    std::thread worker([&prom] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        prom.set_value(11);
    });
    int result = fut.get();
    std::cout << "worker promise result :" << result << std::endl;
    worker.join();
}

void testSharedFuture() {
    std::promise<int> prom;
    std::shared_future<int> fut = prom.get_future().share();

    std::thread sender([&prom]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        prom.set_value(11);
    });

    ThreadGuard senderGuard(sender);

    std::thread reciver1(std::thread([&fut]() {
        std::cout << "reciver1 recive :" << fut.get() << std::endl;
    }));
    ThreadGuard reciverGuard1(reciver1);

    std::thread reciver2(std::thread([&fut]() {
        std::cout << "reciver2 recive :" << fut.get() << std::endl;
    }));
    ThreadGuard reciverGuard2(reciver2);
}

/*
std::barrier (C++20): 固定数量线程的“阶段同步点”。
- 每个阶段：所有参与者都 arrive 后，才进入下一阶段。
- 与 std::latch 区别：latch 一次性倒数；barrier 可重复多阶段（completion 返回 max()）。
- 与 condition_variable 区别：barrier 适合“全员到齐再继续”，无需手写 predicate/notify。

典型场景：并行仿真时间步、分阶段 MapReduce、渲染 pass 间同步。
*/
void testBarrier() {
    PRINT_FUNC_NAME();

    constexpr int kNumWorkers = 4;
    constexpr int kPhases = 3;

    // 每个 worker 在各阶段的局部结果，barrier 保证阶段间可见性
    std::vector<int> partial(kNumWorkers, 0);
    int phase_id = 0;

    // completion 在当前阶段全员到齐后由“最后一个到达的线程”调用一次
    auto on_phase_done = [&phase_id]() noexcept -> std::ptrdiff_t {
        std::cout << "barrier phase " << phase_id << " complete" << std::endl;
        ++phase_id;
        // 返回 max() 表示用相同 expected 重置，进入下一阶段
        return std::barrier<>::max();
    };

    std::barrier sync(kNumWorkers, on_phase_done);

    std::vector<std::thread> workers;
    workers.reserve(kNumWorkers);
    for (int worker_id = 0; worker_id < kNumWorkers; ++worker_id) {
        workers.emplace_back([&, worker_id]() {
            for (int phase = 0; phase < kPhases; ++phase) {
                // 模拟各 worker 在本阶段的工作（耗时不同）
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(50 * (worker_id + 1)));
                partial[worker_id] = (worker_id + 1) * (phase + 1);
                std::cout << std::this_thread::get_id() << " worker "
                          << worker_id << " done phase " << phase
                          << ", partial=" << partial[worker_id] << std::endl;

                // arrive_and_wait = arrive() + wait()，阻塞直到本阶段全员到齐
                sync.arrive_and_wait();
            }
        });
    }

    for (auto &t : workers) {
        t.join();
    }

    int total = 0;
    for (int v : partial) {
        total += v;
    }
    std::cout << "barrier final total=" << total << std::endl;
}

/*
std::latch (C++20): 一次性倒数同步。
- 构造时设定 expected，每次 count_down() 减 1，减到 0 时唤醒所有 wait()。
- 与 std::barrier 区别：不可重置，只适用“等 N 件事完成一次”。
- 与 condition_variable 区别：无需 mutex + predicate，fork-join 语义更直接。

典型场景：并行批任务汇总、等待 N 个 worker 初始化就绪、单阶段 fork-join。
*/
void testLatch() {
    PRINT_FUNC_NAME();

    constexpr int kNumTasks = 4;
    // 倒数至 0 时释放 wait()；latch 不可复用，多阶段请用 barrier
    std::latch all_done(kNumTasks);
    std::vector<int> results(kNumTasks, 0);

    std::vector<std::thread> tasks;
    tasks.reserve(kNumTasks);
    for (int task_id = 0; task_id < kNumTasks; ++task_id) {
        tasks.emplace_back([&, task_id]() {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(80 * (task_id + 1)));
            results[task_id] = (task_id + 1) * 10;
            std::cout << std::this_thread::get_id() << " task " << task_id
                      << " result=" << results[task_id] << std::endl;
            all_done.count_down();
        });
    }

    // 主线程阻塞，直到全部子任务 count_down 完毕
    all_done.wait();

    int sum = 0;
    for (int v : results) {
        sum += v;
    }
    std::cout << "latch all tasks done, sum=" << sum << std::endl;

    for (auto &t : tasks) {
        t.join();
    }
}

/*
线程池批任务：少量 worker 处理大量 task。
- latch(kNumTasks)：等待 12 次 task 完成，而不是 join 4 个 worker。
- join 只能等线程结束；worker 会反复取任务，任务完成与线程生命周期解耦。
- 渲染/引擎常见模式：一帧提交 N 个 job，worker 池执行，主线程 wait 后 present。
*/
void testLatchTaskBatch() {
    PRINT_FUNC_NAME();

    constexpr int kNumWorkers = 4;
    constexpr int kNumTasks = 12;

    std::mutex queue_mutex;
    std::queue<int> pending_tasks;
    for (int task_id = 0; task_id < kNumTasks; ++task_id) {
        pending_tasks.push(task_id);
    }

    std::latch tasks_done(kNumTasks);
    std::vector<int> results(kNumTasks, 0);

    std::vector<std::thread> workers;
    workers.reserve(kNumWorkers);
    for (int worker_id = 0; worker_id < kNumWorkers; ++worker_id) {
        workers.emplace_back([&, worker_id]() {
            while (true) {
                int task_id = -1;
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    if (pending_tasks.empty()) {
                        break;
                    }
                    task_id = pending_tasks.front();
                    pending_tasks.pop();
                }

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(30 * (task_id % 3 + 1)));
                results[task_id] = (task_id + 1) * (task_id + 1);
                std::cout << std::this_thread::get_id() << " worker "
                          << worker_id << " finished task " << task_id
                          <<"."<< std::endl;

                // 报告 task 完成；worker 线程可能继续处理下一个 task
                tasks_done.count_down();
            }
        });
    }

    // 等 12 个 task，不是等 4 个 thread——这是 latch 相对 join 的关键差异
    tasks_done.wait();

    int sum = 0;
    for (int v : results) {
        sum += v;
    }
    std::cout << "latch task batch done, " << kNumTasks << " tasks by "
              << kNumWorkers << " workers, sum=" << sum << std::endl;

    for (auto &t : workers) {
        t.join();
    }
}

}  // namespace synchronization
