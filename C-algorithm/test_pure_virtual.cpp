/*
 * If a pure_virtual_function had a definition in base class, it can be invoked and executed!
 * so, in c++, pure_virtual_function only means derived classes must override it,
 * don't means the base class can not to define it.
*/
#include <iostream>
#include <string>

using namespace std;

class A
{
public:
    A();
    virtual ~A();
    
    virtual void fund1(int t) = 0;
    virtual int c3(int t) = 0;
};

A::A(){}
A::~A(){}


void A::fund1(int t)
{
    cout << "In A::fund1" << endl;
    t++;
    c3(t);  // Oh, which c3() would be invoked?! A::c3 or B::c3 ? Guess...
}


int A::c3(int t)
{
    cout << "In A::c3 " << t << endl;
    return t;
}

class B: public A
{
public:
    virtual void fund1(int t) 
    {
        cout << "In B::fund1 " << t << endl; 
        A::fund1(t);  // call fund1 in base class, and it's a pure virtual function.
    }
    virtual int c3(int t) 
    {
        cout << "In B::c3 " << t << endl;
        return t;
    }
};

int main()
{
    B().fund1(999);
    
    return 0;
}

/** output **
In B::fund1 999
In A::fund1
In B::c3 1000
**/

