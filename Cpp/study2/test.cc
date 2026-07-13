#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;
// struct Port
// {
//     int x;
//     int y;
// };
// int main()
// {
//     // int arr[10]={1,2,34,4};
//     // Port p1={1,2};
//     // //一切定义皆可{}
//     // int x1={2};
//     // std::cout<< x1 <<std::endl;

//     // 左值：可以取地址
//     // 以下的p、b、c、*p、s、s[0]就是常⻅的左值
//     int *p = new int(0);
//     int b = 1;
//     const int c = b;
//     *p = 10;
//     string s("111111");
//     s[0] = 'x';
//     cout << &c << endl;
//     cout << (void *)&s[0] << endl;
//     // 右值：不能取地址
//     double x = 1.1, y = 2.2;
//     // 以下⼏个10、x + y、fmin(x, y)、string("11111")都是常⻅的右值
//     10;
//     x + y;
//     // fmin(x, y);
//     string("11111");
//     // cout << &10 << endl;
//     // cout << &(x+y) << endl;
//     // cout << &(fmin(x, y)) << endl;
//     // cout << &string("11111") << endl;

//     // 左值引⽤不能直接引⽤右值，但是const左值引⽤可以引⽤右值
//     const int &rx1 = 10;
//     const double &rx2 = x + y;
//     //const double &rx3 = fmin(x, y);
//     const string &rx4 = string("11111");
//     // 右值引⽤不能直接引⽤左值，但是右值引⽤可以引⽤move(左值)
//     int &&rrx1 = move(b);
//     int *&&rrx2 = move(p);
//     int &&rrx3 = move(*p);
//     string &&rrx4 = move(s);
//     string &&rrx5 = (string &&)s;
//     return 0;
// }

// void f(int &x)
// {
//     std::cout << "左值引⽤重载 f(" << x << ")\n";
// }
// void f(const int &x)
// {
//     std::cout << "到 const 的左值引⽤重载 f(" << x << ")\n";
// }
// void f(int &&x)
// {
//     std::cout << "右值引⽤重载 f(" << x << ")\n";
// }
// int main()
// {
//     int i = 1;
//     const int ci = 2;
//     f(i);            // 调⽤ f(int&)
//     f(ci);           // 调⽤ f(const int&)
//     f(3);            // 调⽤ f(int&&)，如果没有 f(int&&) 重载则会调⽤ f(const int&)
//     f(std::move(i)); // 调⽤ f(int&&)
//     // 右值引⽤变量在⽤于表达式时是左值
//     int &&x = 1;
//     f(x);            // 调⽤ f(int& x)
//     f(std::move(x)); // 调⽤ f(int&& x)
//     return 0;
// }

// 引用折叠
//  template <class T>
//  void Function(T &&t)
//  {
//      int a = 0;
//      T x = a;
//      // x++;
//      cout << &a << endl;
//      cout << &x << endl
//           << endl;
//  }
//  int main()
//  {
//      // 10是右值，推导出T为int，模板实例化为void Function(int&& t)
//      Function(10); // 右值
//      int a;
//      // a是左值，推导出T为int&，引⽤折叠，模板实例化为void Function(int& t)
//      Function(a); // 左值
//      // std::move(a)是右值，推导出T为int，模板实例化为void Function(int&& t)
//      Function(std::move(a)); // 右值
//      const int b = 8;
//      // b是左值，推导出T为const int&，引⽤折叠，模板实例化为void Function(const int&t)
//      // 所以Function内部会编译报错，x不能++
//      Function(b); // const 左值
//      // std::move(b)右值，推导出T为const int，模板实例化为void Function(const int&&t)
//      // 所以Function内部会编译报错，x不能++
//      Function(std::move(b)); // const 右值
//      return 0;
//  }

// int main()

// {
//     //lambda[捕捉列表](列表参数)->返回时{函数体}
//     //本质是仿函数
//     auto add1 = [](int x, int y) -> int
//     { return x + y; };
//     std::cout << add1(1, 2) << std::endl;
//     return 0;
// }
// void Print(int n, int i)
// {
//     for (; i < n; i++)
//     {
//         cout << this_thread::get_id() << ":" << i << endl;
//     }
//     cout << endl;
// }
// int main()
// {
//     thread t1(Print, 20, 30);
//     thread t2(Print, 13, 12);
//     mutex mut;
//     lock_guard<mutex> lock(mut);
//     cout << t1.get_id()<<endl;
//     cout << t2.get_id()<<endl;

//     t1.join();
//     t2.join();
//     return 0;
// }
std::mutex foo, bar;
void task_a()
{
    // foo.lock(); bar.lock(); // replaced by:
    std::lock(foo, bar);//防止死锁
    std::cout << "task a\n";
    foo.unlock();
    bar.unlock();
}
void task_b()
{
    // bar.lock(); foo.lock(); // replaced by:
    std::lock(bar, foo);
    std::cout << "task b\n";
    bar.unlock();
    foo.unlock();
}
int main()
{
    foo.lock();
    std::thread th1(task_a);
    std::thread th2(task_b);
    std::cout << "xxxxxx" << std::endl;
    bar.lock();
    foo.unlock();
    std::cout << "yyyyyy" << std::endl;
    bar.unlock();
    th1.join();
    th2.join();
    return 0;
}