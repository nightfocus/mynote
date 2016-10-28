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
    fu(vh);  // vh�����÷�ʽ���룬fu�ڲ����Ըı���ֵ.
}

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
    myfun(x);
    
    // 5. ����pthread�̣߳���ֱ�ӱ�д��������.
    pthread_t t1;
    pthread_create(&t1, NULL, [](void* data)->void*
    {
        int pi = *(int*)data;
        cout <<  pi << " in thread." << endl;
    }, &i);
    
    sleep(1);
    return 0;
}
