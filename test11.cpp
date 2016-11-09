/*
    C++ 11标准的一些示例
    
    g++ -o test11 test11.cpp -std=c++11 -lpthread -Wall
    
    c++ 11 feature test.
    gcc version 4.8.4 pass through.
*/

#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <chrono>  // c++11. all below.
#include <thread>
#include <atomic>
#include <mutex>  


using namespace std;
using namespace chrono;

pthread_mutex_t mutex1;
mutex mt11;

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

void autoincrease_fun1(int& cnt)
{
    for(int i=0; i<9000000; i++)
        cnt++;
}

void autoincrease_fun2(atomic_int& cnt)
{
    for(int i=0; i<9000000; i++)
        cnt++;
}

void autoincrease_fun3(int& cnt)
{
    for(int i=0; i<9000000; i++)
    {
        std::lock_guard<std::mutex> lck (mt11); // 使用这个耗时略高于pthread_mutex_lock/unlock.
        // pthread_mutex_lock(&mutex1);
        cnt++;
        // pthread_mutex_unlock(&mutex1);
    }
}


class A
{
public:
    virtual void fun1(int si) { cout << "In A::fun1(): " << si << endl; }
    virtual void fun2() final {} // final 明确表示这个函数不能被派生类覆盖
    virtual ~A() { cout << "A:~A();" << endl; }
};

class B: public A
{
public:
    // override 明确表示fun1这个函数需覆盖基类的。如果没有可覆盖的，会编译失败。
    virtual void fun1(int si) override { cout << "In B::fun1(): " << si << endl; }
    ~B() { cout << "B:~B();" << endl; }
};

void* thread1_fun(shared_ptr<A> pa)
{
    pa->fun1(90101);
    return NULL;
}


template <typename ... T> void DummyWrapper(T... t){};

template <class T>
T unpacker(const T& t)
{
    cout<<','<<t;
    return t;
}

template <typename T, typename... Args>
void write_line(const T& t, const Args& ... data)
{
    cout << t;
    // 直接用unpacker(data)...是非法的，(可以认为直接逗号并列一堆结果没有意义)，需要包裹一下，就好像这些结果还有用
    DummyWrapper(unpacker(data)...); 
    cout << '\n';
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
    // 完整写法是：[&]()->void{...};
    auto lambda_func = [&] { i = 10; cout << "i turn to " << i << endl;};
    lambda_func();

    cout << "---------------------------------------------" << endl << endl;
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
    
    cout << "---------------------------------------------" << endl << endl;
    // 4. 将匿名函数用作回调函数传递
    // 写法1： 示例的函数参数：int&；返回值由编译器自己推断
    auto x = [](int& j){ j = 8887; cout << "hello: " << j << endl;};
    // 写法2： 示例的函数参数：int&；返回值指定为void
    // auto x = [](int& j)->void{ j = 1; cout << "hello: " << j << endl;};
    regcb_fun(x);
    
    cout << "---------------------------------------------" << endl << endl;
    // 5. 创建POSIX pthread线程，并直接编写线程函数.
    pthread_t t1;
    pthread_create(&t1, NULL, [](void* data)->void*
    {
        int pi = *(int*)data;
        cout <<  pi << " in thread." << endl;
        return nullptr;
    }, &i);
    
    cout << "---------------------------------------------" << endl << endl;
    // 6. 使用线程类创建线程, 手册：http://www.cplusplus.com/reference/thread/thread/
    int x1=90;
    int x2 = 100;
    thread td(thread11_fun, x1, std::ref(x2)); // 两个参数，一个是值传递，一个是引用传递
    td.join();
    
    cout << "---------------------------------------------" << endl << endl;
    // 7. 使用对象的非static成员函数创建线程
    CA ca;
    // 写法1: 第一个参数用std::mem_fn标示这是一个成员函数，第二个参数传入是使用哪个对象的成员函数
    // thread mtd1(std::mem_fn(&CA::fun1), ca, x1);
    // 写法2：截取外部作用域中所有变量，并拷贝一份在函数体中使用，但是对ca使用引用
    thread mtd1([=, &ca](){ca.fun1(x1);});
    mtd1.detach(); // 必须detach或join线程对象mtd1，否则在mtd1的作用域结束时，会抛出异常导致崩溃.    
        
    // 8. 时钟
    std::chrono::seconds asec(3000); // 定义一个 3000秒的chrono::seconds对象，精度是秒
    std::chrono::minutes b = std::chrono::duration_cast<std::chrono::minutes>(asec); // 将这个对象转换成以精度是分钟的对象
    cout << asec.count() << " seconds = " << b.count() << " minutes." << endl;  
    
    auto c = std::chrono::duration<int,std::milli>(5000); // 精度是毫秒的对象，也可以直接用 std::chrono::milliseconds
    std::chrono::seconds d = std::chrono::duration_cast<std::chrono::seconds>(c); // 转换成精度是秒的对象
    cout << c.count() << " ms = " << d.count() << " sec." << endl;  
    
    cout << "---------------------------------------------" << endl;
    // 9. atomic使用 及性能对比
    // case 1: 使用10线程不加锁对int进行自增. 速度最快,但结果不对。
    {
        auto start1 = system_clock::now();
        int cnt = 0;
        std::vector<std::thread> threads;
        for (int i = 1; i <= 10; ++i)
            threads.push_back(std::thread(autoincrease_fun1, ref(cnt)));
        for (auto& th : threads)
            th.join();
        auto end1 = system_clock::now();
        auto duration = duration_cast<chrono::milliseconds>(end1 - start1);
        cout << "cnt1: " << cnt << ", duration: " << double(duration.count()) << " ms." << endl;
    }
    
    // case 2: 使用atomic_int原子变量. 耗时约是case1的3-5倍，结果正确。
    {
        auto start1 = system_clock::now();
        atomic_int cnt(0);
        std::vector<std::thread> threads;
        for (int i = 1; i <= 10; ++i)
            threads.push_back(std::thread(autoincrease_fun2, ref(cnt)));
        for (auto& th : threads)
            th.join();
        auto end1 = system_clock::now();
        auto duration = duration_cast<chrono::milliseconds>(end1 - start1);
        cout << "cnt2: " << cnt << ", duration: " << double(duration.count()) << " ms." << endl;
    }
    
    // case 3: 使用c++11的互斥锁。 耗时约是case2的5-7倍，结果正确。
    {
        auto start1 = system_clock::now();
        int cnt = 0;
        std::vector<std::thread> threads;
        for (int i = 1; i <= 10; ++i)
            threads.push_back(std::thread(autoincrease_fun3, ref(cnt)));
        for (auto& th : threads)
            th.join();
        auto end1 = system_clock::now();
        auto duration = duration_cast<chrono::milliseconds>(end1 - start1);
        cout << "cnt3: " << cnt << ", duration: " << double(duration.count()) << " ms." << endl;
    }

    // 10. 智能指针. shared_ptr
    shared_ptr<B> pb2 = nullptr;
    {
        shared_ptr<B> pb = make_shared<B>();
        pb2 = pb;
        cout << "out of pb lifetime, but don't invoke B::~B()" << endl;
    }
    // 将shared_ptr作为线程参数传递
    {
        shared_ptr<A> pa1 = make_shared<A>();
        thread td(thread1_fun, pa1);
        td.detach();
        // 在线程结束后，并且pa1生命周期结束，会调用A的析构函数.
    }
    // 定义一个指针对象spv，指向的vector<int>有10个元素，每个元素值为987
    auto spv = std::make_shared<std::vector<int>>(12, 987);
    cout << "spv.size: ()" << spv->size() << endl;
    for(auto it : *spv)
        cout << "  " << it << endl;    
    
    // 变长参数模板
    int ji = 109;
    string js = "alinjefo ";
    write_line(ji, js);
    
    sleep(1);
    cout << "byebye." << endl;
    return 0;
}
