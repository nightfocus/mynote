/**
    两个算法示例：
    1. 有一个n个元素的数组，除了一个元素只出现一次外，其他元素都出现两次，让你找出这个只出现一次的元素是几。
    2. 同上，但有两个元素出只出一次，其他元素都出现两次。
    
    
    g++ -o finddifferent finddifferent.c -Wall
*/


#include <stdio.h>

// 数组1， 71是这个不同的元素
int n1[9] = {34, 65, 109, 110, 109, 71, 110, 34, 65};

// 数组2， 71, 125 是这两个不同的元素
int n2[10] = {34, 110, 65, 109, 109, 71, 110, 34, 125, 65};


void find1()
{
    int rs = 0;
    for(unsigned int i=0; i<sizeof(n1)/sizeof(int); i++)
    {
        rs = rs^n1[i]; // 把所有数字全部异或，根据异或的特性，相同数字进行异或，结果为0。
    }
    printf("n1: %d\n", rs);
}

void find2()
{
    // step 1: 把所有数字全部异或，同find1()，得出的结果rs实际上是这两个元素的异或结果。
    int rs = 0;
    for(unsigned int i=0; i<sizeof(n2)/sizeof(int); i++)
    {
        rs = rs^n2[i]; 
    }
    
    // step 2: 这两个元素至少有1bit是不同的，所以rs至少有1bit是1，找出它的位置j先.
    int j=0;
    int tmp = 0x00;
    while(1)
    {
        tmp = rs&(0x01<<j);
        if(tmp > 0)
            break;
        j++;
    };
    
    // step 3: 所有的元素中，j位要么是1，要么是0，按这个规则将所有元素分开，
    // j位为1的进行异或，j位为0的进行异或，这样就得出了这两个元素了。
    int rs1=0;
    int rs2=0;
    for(unsigned int i=0; i<sizeof(n2)/sizeof(int); i++)
    {
        if(n2[i]&(0x01<<j))
            rs1 = rs1^n2[i];
        else
            rs2 = rs2^n2[i];
    }
    
    printf("n2: %d, %d\n", rs1, rs2);
    
    return;
}


int main()
{
    find1();
    printf("----------------------------------\n");
    find2();
    
    return 0;
}
