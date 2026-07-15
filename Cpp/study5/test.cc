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

#include <iostream>
using namespace std;
class Date
{
public:
    constexpr Date(int year, int month, int day)
        : _year(year), _month(month), _day(day)
    {
        // cout << "constexpr Date(int year, int month, int day)" << endl;
    }
    constexpr int GetYear() const
    {
        return _year;
    }

private:
    int _year;
    int _month;
    int _day;
};
template <typename T>
constexpr T Func(T t)
{
    return t;
}
int main()
{
    int x = 2025;
    // constexpr Date d0(x, 9, 8); // 报错
    constexpr Date d1(2025, 9, 8);
    constexpr int y = d1.GetYear();
    Date d2(x, 8, 11);
    int z = d2.GetYear();
    string ret1 = Func("111111"); // 普通函数
    constexpr int ret2 = Func(10);
    return 0;
}