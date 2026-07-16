#include <iostream>
// int size()
// {
//     int n = 10;
//     return n;
// }
// int main()
// {
//     const int a = 1;      // a是常量表达式
//     const int b = a + 1;  // b是常量表达式
//     int c = 1;            // c不是常量表达式
//     const int d = c;      // d不是常量表达式
//     const int e = size(); // e不是常量表达式
//     // 常量表达式可以做数组⼤⼩，vs不⽀持变⻓数组，所以这⾥数组⼤⼩必须是编译时确认的
//     int arr[a];
//     constexpr int aa = 1;
//     constexpr int bb = aa + 1;
//     // constexpr int cc = c; // 报错
//     // constexpr int cc = size(); // 报错
//     // constexpr int* p1 = &d; //报错，权限放⼤了，constexpr修饰的是p1本⾝
//     const int *p2 = &d;
//     //constexpr const int *p3 = &d; // constexpr修饰的是p3本⾝，const修饰*p3
//     return 0;
// }
// using namespace std;
// constexpr int size()
// {
//     return 10;
// }
// constexpr int func(int x)
// {
//     return 10 + x;
// }
// constexpr int factorial(int n)
// {
//     return n <= 1 ? 1 : n * factorial(n - 1);
// }
// constexpr int fxx(int x)
// {
//     int i = x;
//     i++;
//     // cout << i << endl;
//     return 10 + x;
// }
// int main()
// {
//     // 编译时，N会被直接替换为10，constexpr函数默认就是inline
//     constexpr int N1 = size();
//     int arr1[N1];
//     // func传10时，func函数返回值是常量表达式，所以N2是常量表达式
//     constexpr int N2 = func(10);
//     int arr2[N2];
//     // func传10时，func函数返回值是常量表达式，所以N2是常量表达式
//     int i = 10;
//     // constexpr int N3 = func(i);         // 报错func返回的不是常量表达式
//     int N4 = func(i);                   // 不报错constexpr函数返回的不⼀是常量表达式
//     constexpr int fact5 = factorial(5); // 编译时计算
//     // constexpr修饰的函数可以有⼀些其他语句，但是这些语句运⾏时可以不执⾏任何操作就可以
//     //  如类型别名、空语句、using声明等
//     constexpr int N5 = fxx(10);
//     return 0;
// }

// #include <iostream>
// using namespace std;
// class Date
// {
// public:
//     constexpr Date(int year, int month, int day)
//         : _year(year), _month(month), _day(day)
//     {
//         // cout << "constexpr Date(int year, int month, int day)" << endl;
//     }
//     constexpr int GetYear() const
//     {
//         return _year;
//     }

// private:
//     int _year;
//     int _month;
//     int _day;
// };
// template <typename T>
// constexpr T Func(T t)
// {
//     return t;
// }
// int main()
// {
//     int x = 2025;
//     // constexpr Date d0(x, 9, 8); // 报错
//     constexpr Date d1(2025, 9, 8);
//     constexpr int y = d1.GetYear();
//     Date d2(x, 8, 11);
//     int z = d2.GetYear();
//     string ret1 = Func("111111"); // 普通函数
//     constexpr int ret2 = Func(10);
//     return 0;
// }

// #include <iostream>

// // C++14 风格：使用循环和局部变量
// 要求所有操作在编译期可执行（如不能调用非 constexpr 函数，不能有 new/delete
// constexpr int factorial(int n) {
//     int result = 1;          // 局部变量
//     for (int i = 2; i <= n; ++i) {
//         result *= i;         // 修改局部变量
//     }
//     return result;           // 多个 return 也可以
// }

// int main() {
//     // 编译期计算，结果作为数组大小（必须常量）
//     static_assert(factorial(5) == 120, "编译期计算错误");

//     // 也可以运行时调用，完全正常
//     int x = 6;
//     std::cout << "factorial(6) = " << factorial(x) << '\n'; // 运行时计算
//     return 0;
// }
// #include <iostream>
// #include <vector>
// #include <array>
// using namespace std;
// struct Point
// {
//     constexpr Point(double x, double y) : x(x), y(y) {}
//     double x, y;
// };
// constexpr Point midpoint(Point a, Point b)
// {
//     return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
// }
// constexpr std::array<int, 5> createArray()
// {
//     std::array<int, 5> arr{};
//     for (size_t i = 0; i < arr.size(); ++i)
//     {
//         arr[i] = i * i;
//     }
//     return arr;
// }
// constexpr int fibonacci(int n)
// {
//     return (n <= 1) ? n : (fibonacci(n - 1) + fibonacci(n - 2));
// }
// int main()
// {
//     Point p1 = midpoint({1.1, 1.1}, {2.2, 2.2});
//     constexpr Point p2 = midpoint({1.1, 1.1}, {2.2, 2.2});
//     constexpr std::array<int, 5> a1 = createArray();
//     constexpr int fibArray[] = {
//         fibonacci(0), fibonacci(1), fibonacci(2), fibonacci(3),
//         fibonacci(4), fibonacci(5), fibonacci(6), fibonacci(7)};
//     return 0;
// }
//=======================================C++17======================================
// #include <iostream>

// int main()
// {
//     // 显式声明 constexpr lambda
//     constexpr auto square = [](int x) constexpr
//     { return x * x; };

//     // 编译期使用
//     static_assert(square(5) == 25, "lambda 编译期计算");

//     // 也可以用于数组大小
//     int arr[square(3)] = {0};                                       // arr 大小为 9
//     std::cout << "数组大小: " << sizeof(arr) / sizeof(int) << '\n'; // 输出 9
//                                                                     // 也可以隐式成为 constexpr（如果满足条件，但显式更安全）
//     auto add = [](int a, int b)
//     { return a + b; }; // C++17 中如果调用时参数是常量表达式，则自动 constexpr

//     // 注意：如果捕获变量，则 lambda 可能不是 constexpr（取决于捕获是否常量）
//     const int a = 10;
//     constexpr auto add_a = [a](int x) constexpr
//     { return x + a; }; // OK，a 是常量
//     static_assert(add_a(5) == 15);
//     return 0;
// }


// #include <iostream>
// #include <type_traits>

// template <typename T>
// void printTypeInfo() {
//     if constexpr (std::is_integral_v<T>) {
//         std::cout << "T is integral\n";
//     } else if constexpr (std::is_floating_point_v<T>) {
//         std::cout << "T is floating point\n";
//     } else {
//         std::cout << "T is other type\n";
//     }
// }

// int main() {
//     printTypeInfo<int>();    // 输出 "T is integral"
//     printTypeInfo<double>(); // 输出 "T is floating point"
//     printTypeInfo<std::string>(); // 输出 "T is other type"
//     return 0;
// }


//=========================================C++20=======================
// #include <iostream>

// struct Base {
//     constexpr virtual int get() const { return 0; } // 虚函数 constexpr
// };

// struct Derived : Base {
//     constexpr int get() const override { return 42; }
// };

// int main() {
//     constexpr Derived d;
//     const Base* ptr = &d;
//     int val = ptr->get();
//     std::cout << val << '\n';
//     return 0;
// }

// #include <memory>

// constexpr int createAndUse() {
//     int* p = new int(100);   // 编译期分配
//     int val = *p;
//     delete p;                // 编译期释放
//     return val;
// }

// int main() {
//     constexpr int result = createAndUse();   // 100
//     std::cout << result << '\n';
//     return 0;
// }

