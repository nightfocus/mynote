/*
    g++ -o test11 test11.cpp -std=c++11 -lpthread
    
    c++ 11 feature test.
    gcc version 4.8.4 pass through.
*/
#include <iostream>
#include <string>
#include <map>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void myfun(void(*fu)(int& i))
{
    int vh = 999; 
    fu(vh);  // vh是引用方式传入，fu内部可以改变其值.
}

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
    myfun(x);
    
    // 5. 创建pthread线程，并直接编写函数主体.
    pthread_t t1;
    pthread_create(&t1, NULL, [](void* data)->void*
    {
        int pi = *(int*)data;
        cout <<  pi << " in thread." << endl;
    }, &i);
    
    sleep(1);
    return 0;
}
