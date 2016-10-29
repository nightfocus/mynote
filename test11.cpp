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
    fu(vh);  // vh�����÷�ʽ���룬fu�ڲ����Ըı���ֵ.
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
    // 1. lambda ���ʽ������������. 
    /*
        [] : �ղ�ץ����,���� lambda body �����ʱպϷ�Χ(enclosing scope)���κα���. 
        [&] : �����õķ�ʽ���ʱպϷ�Χ�ڵ�ǰ������������. 
        [=] : ��ֵ�ķ�ʽ���ʱպϷ�Χ�ڵ�ǰ���������ı���. 
        [this] : ������ʵ����thisָ��.
    */
    int i = 1024;
    // ����д���ǣ�[&]()->void{...};
    auto lambda_func = [&] { i = 10; cout << "i turn to " << i << endl;};
    lambda_func();

    // 2. stl��������ʱ��ֵ
    map<int, string> mm = {{23, "one string."}, {100, "greedy."}};
    
    // 3. �µ�iteratorѭ����ʽ
    for(auto& i:mm) // ���÷�ʽ
    {
        cout << i.second << endl;
        i.second += " postfix";  // �ı�ԭ�е�value
    }
    for(auto i:mm) // ����һ�Σ��Դ�ֵ��ʽ
    {
        cout << "the second: " << i.second << endl;
    }    
    
    // 4. ���������������ص���������
    // д��1�� ʾ���ĺ���������int&������ֵ�ɱ������Լ��ƶ�
    auto x = [](int& j){ j = 8887; cout << "hello: " << j << endl;};
    // д��2�� ʾ���ĺ���������int&������ֵָ��Ϊvoid
    // auto x = [](int& j)->void{ j = 1; cout << "hello: " << j << endl;};
    regcb_fun(x);
    
    // 5. ����POSIX pthread�̣߳���ֱ�ӱ�д�̺߳���.
    pthread_t t1;
    pthread_create(&t1, NULL, [](void* data)->void*
    {
        int pi = *(int*)data;
        cout <<  pi << " in thread." << endl;
    }, &i);
    
    // 6. ʹ���߳��ഴ���߳�, �ֲ᣺http://www.cplusplus.com/reference/thread/thread/
    int x1=90;
    int x2 = 100;
    thread td(thread11_fun, x1, std::ref(x2)); // ����������һ����ֵ���ݣ�һ�������ô���
    td.join();
    
    // 7. ʹ�ö���ķ�static��Ա���������߳�
    CA ca;
    // д��1: ��һ��������std::mem_fn��ʾ����һ����Ա�������ڶ�������������ʹ���ĸ�����ĳ�Ա����
    // thread mtd1(std::mem_fn(&CA::fun1), ca, x1);
    // д��2����ȡ�ⲿ�����������б�����������һ���ں�������ʹ�ã����Ƕ�caʹ������
    thread mtd1([=, &ca](){ca.fun1(x1);});
    mtd1.detach(); // ����detach��join�̶߳���mtd1��������mtd1�����������ʱ�����׳��쳣���±���.    
    
    
    sleep(1);
    cout << "byebye." << endl;
    return 0;
}
