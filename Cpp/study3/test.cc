// #include <atomic>
// #include <iostream>
// #include <thread>
// #include <vector>
// using namespace std;

// atomic<int> acnt;

// int cnt;
// void Add1(atomic<int> &cnt)
// {
//     int old = cnt.load();
//     // 如果cnt的值跟old相等，则将cnt的值设置为old+1，并且返回true，这组操作是原⼦的。
//     // 那么如果在load和compare_exchange_weak操作之间cnt对象被其他线程改了
//     // 则old和cnt不相等，则将old的值改为cnt的值，并且返回false。
//     while (!atomic_compare_exchange_weak(&cnt, &old, old + 1));
//     // while (!cnt.compare_exchange_weak(old, old + 1));
// }
// void f()
// {
//     for (int n = 0; n < 100000; ++n)
//     {
//         ++acnt;
//         // Add1的⽤CAS模拟atomic的operator++的原⼦操作
//         // Add1(acnt);
//         ++cnt;
//     }
// }
// int main()
// {
//     std::vector<thread> pool;
//     for (int n = 0; n < 4; ++n)
//         pool.emplace_back(f);
//     for (auto &e : pool)
//         e.join();
//     cout << "原⼦计数器为 " << acnt << '\n'
//          << "⾮原⼦计数器为 " << cnt << '\n';
//     return 0;
// }

// #include <atomic>
// #include <thread>
// #include <iostream>

// std::atomic<bool> flag(false);

// void writer() {
//     // 存储 true，使用默认顺序（顺序一致）
//     flag.store(true);
//     std::cout << "Writer: flag set to true\n";
// }

// void reader() {
//     // 循环等待 flag 变为 true
//     while (!flag.load()) {
//         // 忙等待，实际中可加入小延迟
//     }
//     std::cout << "Reader: flag is true, proceeding...\n";
// }

// int main() {
//     std::thread t1(reader);
//     std::thread t2(writer);
//     t1.join();
//     t2.join();
//     return 0;
// }

// #include <atomic>
// #include <iostream>
// #include <thread>

// std::atomic<int> data(10);

// void thread_func() {
//     int old = data.exchange(20); // 原子地将 data 设为20，并返回原值10
//     std::cout << "Old value: " << old << ", new value: " << data.load() << '\n';
// }

// int main() {
//     std::thread t(thread_func);
//     t.join();
//     return 0;
// }

#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

class SpinLock
{
    std::atomic_flag flag = ATOMIC_FLAG_INIT; // 初始化为 false
public:
    void lock()
    {
        // 循环尝试设置 flag，直到成功（原来为 false，我们设置为 true）
        while (flag.test_and_set(std::memory_order_acquire))
        {
            // 自旋等待
        }
    }
    void unlock()
    {
        flag.clear(std::memory_order_release); // 释放锁
    }
};

SpinLock spin;
int shared_data = 0;

void worker()
{
    for (int i = 0; i < 1000; ++i)
    {
        spin.lock();
        ++shared_data;
        spin.unlock();
    }
}

// int main() {
//     std::vector<std::thread> threads;
//     for (int i = 0; i < 10; ++i) {
//         threads.emplace_back(worker);
//     }
//     for (auto& t : threads) t.join();
//     std::cout << "shared_data = " << shared_data << '\n'; // 应为 10000
//     return 0;
// }
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>

std::deque<int> q;
std::mutex mtx;
std::condition_variable cv;
const int MAX_ITEMS = 10; // 只是为了演示，非必须

void producer(int id)
{
    for (int i = 0; i < 10; ++i)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push_back(i);
            std::cout << "生产者 " << id << " 生产了 " << i
                      << "，当前队列大小 = " << q.size() << std::endl;
        }
        cv.notify_one(); // 通知消费者（不一定要持有锁）
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(int id)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        // // 使用谓词等待：队列不为空时才返回
        // cv.wait(lock, []
        //         { return !q.empty(); });
        //循环等待
        while (q.empty())
        {
            cv.wait(lock);
        }

        // 取出队首元素
        int value = q.front();
        q.pop_front();
        std::cout << "消费者 " << id << " 消费了 " << value
                  << "，当前队列大小 = " << q.size() << std::endl;

        // 这里可以处理 value...
        // 为了演示，我们生产10个后退出，但消费者在消费完所有后仍会阻塞（因为队列空且没有生产）
        // 更好的方式是设置一个结束标志，但为了简洁，我们让消费者在消费一定数量后退出。
        // 实际中可使用 poison pill 或额外标志。
        if (value == 9)
        { // 假设生产者生产0~9，消费到9就退出
            break;
        }
    }
}

int main()
{
    std::thread p(producer, 1);
    std::thread c(consumer, 1);

    p.join();
    c.join();
    return 0;
}