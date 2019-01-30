/*
    g++ -o test11_future test11_future.cpp -std=c++11 -pthread -Wall
*/

#include <iostream>                // std::cout
#include <future>                // std::async, std::future
#include <chrono>                // std::chrono::milliseconds

// a non-optimized way of checking for prime numbers:
bool do_check_prime(int x, int& ry) // 为了体现效果, 该函数故意没有优化.
{
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    for (int i = 2; i < x; ++i)
        if (x % i == 0)
            return false;
    ry = 9527;
    return true;
}

int main()
{
    int y;
    // call function asynchronously:
    std::future < bool > fut = std::async(do_check_prime, 194232491, std::ref(y) );

    std::cout << "Checking...\n";
    // fut.wait();

    std::cout << "\n194232491 ";
    if (fut.get()) // guaranteed to be ready (and not block) after wait returns
        std::cout << "is prime." << y << "\n";
    else
        std::cout << "is not prime." << y <<"\n";

    return 0;
}
