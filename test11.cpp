/*
    g++ -o test11 test11.cpp -std=c++11 -lpthread
    
    c++ 11 feature test.
    gcc version 4.8.4 pass through.
*/

#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <thread>  // c++11 thread header file.

using namespace std;

void regcb_fun(void(*fu)(int& i))
{
    int vh = 999; 
    fu(vh);  // vh是引用方式传入，fu内部可以改变其值.
}

void thread11_fun(int a, int& b)
{
    do
    {
        cout << "In thread11_fun: " << a << " , " << b << endl;
        sleep(1);
    }while(false);
    
    return;
}

class CA
{
public:
    void fun1(int b)
    {
        cout << "In CA::fun1: " << b << endl;
    }
    
};

int main()
{
    // 1. lambda 表达式，即匿名函数. 
    /*
        [] : 空捕抓条款,表明 lambda body 不访问闭合范围(enclosing scope)的任何变量. 
        [&] : 以引用的方式访问闭合范围内的前面已声明变量. 
        [=] : 以值的方式访问闭合范围内的前面已声明的变量. 
        [this] : 访问类实例的this指针.
    */
    int i = 1024;
    // 完整写法是：[&]()->void{...};
    auto lambda_func = [&] { i = 10; cout << "i turn to " << i << endl;};
    lambda_func();

    // 2. stl容器声明时赋值
    map<int, string> mm = {{23, "one string."}, {100, "greedy."}};
    
    // 3. 新的iterator循环方式
    for(auto& i:mm) // 引用方式
    {
        cout << i.second << endl;
        i.second += " postfix";  // 改变原有的value
    }
    for(auto i:mm) // 再来一次，以传值方式
    {
        cout << "the second: " << i.second << endl;
    }    
    
    // 4. 将匿名函数用作回调函数传递
    // 写法1： 示例的函数参数：int&；返回值由编译器自己推断
    auto x = [](int& j){ j = 8887; cout << "hello: " << j << endl;};
    // 写法2： 示例的函数参数：int&；返回值指定为void
    // auto x = [](int& j)->void{ j = 1; cout << "hello: " << j << endl;};
    regcb_fun(x);
    
    // 5. 创建POSIX pthread线程，并直接编写线程函数.
    pthread_t t1;
    pthread_create(&t1, NULL, [](void* data)->void*
    {
        int pi = *(int*)data;
        cout <<  pi << " in thread." << endl;
    }, &i);
    
    // 6. 使用线程类创建线程, 手册：http://www.cplusplus.com/reference/thread/thread/
    int x1=90;
    int x2 = 100;
    thread td(thread11_fun, x1, std::ref(x2)); // 两个参数，一个是值传递，一个是引用传递
    td.join();
    
    // 7. 使用对象的非static成员函数创建线程
    CA ca;
    // 写法1: 第一个参数用std::mem_fn标示这是一个成员函数，第二个参数传入是使用哪个对象的成员函数
    // thread mtd1(std::mem_fn(&CA::fun1), ca, x1);
    // 写法2：截取外部作用域中所有变量，并拷贝一份在函数体中使用，但是对ca使用引用
    thread mtd1([=, &ca](){ca.fun1(x1);});
    mtd1.detach(); // 必须detach或join线程对象mtd1，否则在mtd1的作用域结束时，会抛出异常导致崩溃.    
    
    
    sleep(1);
    cout << "byebye." << endl;
    return 0;
}
