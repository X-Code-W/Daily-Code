// #include <iostream>
// #include <future>
// #include <thread>
// #include <chrono>

// // 一个模拟耗时计算的函数
// int slow_square(int x) {
//     std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟耗时 2 秒
//     return x * x;
// }

// int main() {
//     // std::async 启动异步任务，返回 std::future<int>
//     std::future<int> fut = std::async(std::launch::async, slow_square, 5);

//     std::cout << "正在计算 5 的平方，请稍候...\n";

//     // 方式1：阻塞等待并获取结果
//     int result = fut.get();  // 这里会阻塞直到结果就绪
//     std::cout << "结果: " << result << std::endl;

//     // 注意：fut 现在已经无效，不能再调用 get()
//     // 如果想检查是否有效，可以用 valid()
//     std::cout << "future 是否有效? " << (fut.valid() ? "是" : "否") << std::endl;

//     return 0;
// }

// #include <iostream>
// #include <future>
// #include <thread>
// #include <chrono>

// int heavy_compute(int n) {
//     std::cout << "[线程 " << std::this_thread::get_id() << "] 开始计算...\n";
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     return n * 10;
// }

// int main() {
//     // 策略1：async —— 立即在新线程启动
//     std::future<int> fut_async = std::async(std::launch::async, heavy_compute, 7);
//     std::cout << "async 策略：任务已启动（可能在其他线程）\n";

//     // 策略2：deferred —— 不会立即执行
//     std::future<int> fut_deferred = std::async(std::launch::deferred, heavy_compute, 3);
//     std::cout << "deferred 策略：任务尚未执行，只有调用 get() 才会运行\n";

//     // 获取 async 结果（会阻塞，但任务已经在运行）
//     int res1 = fut_async.get();
//     std::cout << "async 结果: " << res1 << std::endl;

//     // 获取 deferred 结果（此时才真正执行 heavy_compute）
//     int res2 = fut_deferred.get();  // 这行会在当前线程执行 heavy_compute
//     std::cout << "deferred 结果: " << res2 << std::endl;

//     return 0;
// }

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

// 工作线程：接收一个 promise 引用，稍后设置值
void do_work(std::promise<int>& prom) {
    try {
        std::cout << "工作线程开始计算...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        int result = 42;  // 模拟计算结果
        prom.set_value(result);  // 设置结果，使 future 就绪
        std::cout << "工作线程已设置值\n";
    } catch (...) {
        // 如果发生异常，可以通过 set_exception 传播
        prom.set_exception(std::current_exception());
    }
}

int main() {
    // 创建 promise 和对应的 future
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    // 启动工作线程，将 promise 移动进去
    // std::thread worker(do_work, std::move(prom));
    std::thread worker(do_work,ref(prom));

    std::cout << "主线程等待结果...\n";
    int value = fut.get();  // 阻塞直到 prom.set_value 被调用
    std::cout << "主线程获得结果: " << value << std::endl;

    worker.join();
    return 0;
}

// #include <iostream>
// #include <future>
// #include <thread>
// #include <functional>

// // 一个普通函数
// int add(int a, int b) {
//     std::cout << "计算 " << a << " + " << b << std::endl;
//     return a + b;
// }

// int main() {
//     // 创建一个 packaged_task，包装 add 函数
//     std::packaged_task<int(int, int)> task(add);  // 参数类型为 int,int，返回 int

//     // 获取关联的 future
//     std::future<int> fut = task.get_future();

//     // 方式1：在另一个线程中执行 task
//     std::thread t(std::move(task), 3, 4);  // 将 task 移动给线程，并传递参数 3,4

//     // 主线程可以同时做其他事...
//     std::cout << "主线程等待结果...\n";

//     // 获取结果（阻塞）
//     int result = fut.get();
//     std::cout << "结果: " << result << std::endl;

//     t.join();

//     // 如果要复用 packaged_task，可以 reset
//     // task.reset();  // 重置后可以再次 get_future 并执行
//     return 0;
// }

// #include <iostream>
// #include <future>
// #include <thread>
// #include <vector>

// int compute() {
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     return 100;
// }

// int main() {
//     // 用一个 promise 来产生结果
//     std::promise<int> prom;
//     std::shared_future<int> shared_fut = prom.get_future().share();  // 转为 shared_future

//     // 启动一个设置结果的线程
//     std::thread setter([&prom]() {
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//         prom.set_value(100);
//         std::cout << "结果已设置\n";
//     });

//     // 启动多个线程，每个都等待并获取 shared_future
//     std::vector<std::thread> waiters;
//     for (int i = 0; i < 5; ++i) {
//         waiters.emplace_back([shared_fut, i]() {
//             std::cout << "线程 " << i << " 等待结果...\n";
//             int val = shared_fut.get();  // 每个线程都可以获取副本
//             std::cout << "线程 " << i << " 得到: " << val << std::endl;
//         });
//     }

//     // 等待所有线程结束
//     for (auto& t : waiters) t.join();
//     setter.join();

//     return 0;
// }


// #include <iostream>
// #include <future>
// #include <thread>
// #include <vector>
// #include <chrono>
// #include <random>

// // 模拟下载函数
// int download_file(const std::string& url) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(500, 1500);
//     int delay = dis(gen);  // 随机 0.5~1.5 秒
//     std::this_thread::sleep_for(std::chrono::milliseconds(delay));
//     std::cout << "下载完成: " << url << " (耗时 " << delay << "ms)\n";
//     return url.length() * 100;  // 模拟返回文件大小
// }

// int main() {
//     // 使用 async 启动多个下载任务
//     std::vector<std::future<int>> download_results;
//     std::vector<std::string> urls = {"http://a.com", "http://b.com", "http://c.com"};

//     for (const auto& url : urls) {
//         download_results.push_back(std::async(std::launch::async, download_file, url));
//     }

//     // 使用 promise 和 future 汇总所有结果
//     std::promise<int> total_promise;
//     std::future<int> total_future = total_promise.get_future();

//     // 启动一个线程计算总和（可以用 packaged_task 也可以）
//     std::thread summer([&download_results, &total_promise]() {
//         int sum = 0;
//         for (auto& fut : download_results) {
//             sum += fut.get();  // 依次等待每个结果
//         }
//         total_promise.set_value(sum);
//     });

//     // 主线程可以显示一个进度指示
//     std::cout << "所有下载任务已启动，等待汇总...\n";
//     int total = total_future.get();
//     std::cout << "总大小: " << total << " 字节\n";

//     summer.join();
//     return 0;
// }