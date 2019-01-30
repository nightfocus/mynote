/*
    一堆数字中，查找不重复的算法
    每个数字使用2个bit来标记，这样一个char可以标记4个数字
    这样如果范围在32位的数字，需要4GB/4 = 1GB的内存即可。
    00是不存在
    01是存在但唯一
    10是存在且重复
    11忽略
*/

#include <iostream>
#include <map>
#include <list>
using namespace std;


void setbit2(unsigned char* x, int tc)
{
    int idx = tc/4;
    int remain = tc%4;
    int bitidx = 0x01 << (remain*2);
    if((x[idx] & bitidx) == 0)
        x[idx] |= bitidx;
    else
        x[idx] |= 0x01 << (remain*2+1);
    return;
}

int main()
{
    // 1024长度，能存储空间为 0 - 4095 的int
    // 40亿Int,则需要开启1GB的空间
    unsigned char x[1024] = {};
    
    setbit2(x, 7);
    setbit2(x, 993);
    setbit2(x, 1020);
    setbit2(x, 6);
    setbit2(x, 323);
    setbit2(x, 477);
    setbit2(x, 8);
    setbit2(x, 323);
    setbit2(x, 6);
    setbit2(x, 1);
    setbit2(x, 8);    
    setbit2(x, 6);
    setbit2(x, 993);

    /*
    for(int i =0; i<sizeof(x); i++)
    {    
        unsigned char z = x[i];
        for(int j=7; j>=0; j--)
            printf("%d", (z&(1<<j))==0 ? 0 : 1);
        printf("\n");
    }
    */
    
    // way 1:
    for(int i =0; i< sizeof(x); i++)
    {    
        unsigned char z = x[i];
        if ((z&0xC0) >> 6 == 0x01)
            printf("%d,", i*4+3);
        if ((z&0x30) >> 4 == 0x01)
            printf("%d,", i*4+2);
        if ((z&0xC) >> 2 == 0x01)
            printf("%d,", i*4+1);
        if ((z&0x3) == 0x01)
            printf("%d,", i*4);     
    }
    printf("\n");
    
    // way 2:
    for(int i =0; i< sizeof(x); i++)
    {    
        unsigned char z = x[i];
        for(int j=6; j>=0; j-=2)
        {
            int t = z & (0x03 << j);
            t = t >> j;
            if(t == 0x01)
                printf("%d,",  i*4+j/2);
        }
    }
    printf("\n");
    
    return 0;
}
