/*
    cpp 关于STL emplace()的一些性能影响
    g++ -o test11_move test11_move.cpp -std=c++11 -pthread -Wall
*/
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

using namespace std;


class Myclass
{
public:
    Myclass(string xx) : x(xx)
    {
        cout << "Myclass(string xx): " << x << endl;
    }
    
    Myclass(const Myclass& mc)
    {
        cout << "Myclass copy construct: " << mc.x << endl;
        this->x = mc.x;
        this->z = mc.z;
    }
    
    Myclass()
    {
        cout << "Myclass default construct" << endl;
    }
    
    void operator= (const Myclass& mc)
    {
        cout << "Myclass operator=: " << mc.x << endl;
        this->x = mc.x;
        this->z = mc.z;
    }
    
public:
    string x;
private:
    int z;
};


int main()
{
    string s1 = "abcd";
    string s2 = std::move(s1);
    cout << "s1: " << s1 << endl;   // s1 is empty.
    
    map<int, Myclass> mmm;
    Myclass z(s2);
    
    cout << "+++++++++++++++++++++ 11" << endl;
    mmm.insert(make_pair(11, z)); // 这会调用两次Myclas的 拷贝构造函数
    cout << "+++++++++++++++++++++ 12" << endl;
    mmm.insert({12, z});          // 同上
    cout << "+++++++++++++++++++++ 13" << endl;
    mmm.emplace(13, z);           // 这会一次 拷贝构造函数
    cout << "+++++++++++++++++++++ 14" << endl;
    mmm.emplace(14, s2);          // 这会调用重载的构造函数
    cout << "+++++++++++++++++++++ 15" << endl;
    mmm[15] = z;                    // 这会调用默认构造函数，和赋值运算符重载函数
    cout << "+++++++++++++++++++++ 16" << endl;
    mmm.insert({16, std::move(z)}); // 这会调用两次Myclas的 拷贝构造函数
    
    cout << "END ----------------" << endl;
    for(auto& it:mmm)
    {
        cout << it.first << " -> " << it.second.x << endl;
    }

}

/*
程序输出：

s1: 
Myclass(string xx): abcd
+++++++++++++++++++++ 11
Myclass copy construct: abcd
Myclass copy construct: abcd
+++++++++++++++++++++ 12
Myclass copy construct: abcd
Myclass copy construct: abcd
+++++++++++++++++++++ 13
Myclass copy construct: abcd
+++++++++++++++++++++ 14
Myclass(string xx): abcd
+++++++++++++++++++++ 15
Myclass default construct
Myclass operator=: abcd
+++++++++++++++++++++ 16
Myclass copy construct: abcd
Myclass copy construct: abcd
END ----------------
11 -> abcd
12 -> abcd
13 -> abcd
14 -> abcd
15 -> abcd
16 -> abcd

*/