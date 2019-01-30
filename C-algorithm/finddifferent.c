/**
    �����㷨ʾ����
    1. ��һ��n��Ԫ�ص����飬����һ��Ԫ��ֻ����һ���⣬����Ԫ�ض��������Σ������ҳ����ֻ����һ�ε�Ԫ���Ǽ���
    2. ͬ�ϣ���������Ԫ�س�ֻ��һ�Σ�����Ԫ�ض��������Ρ�
    
    
    g++ -o finddifferent finddifferent.c -Wall
*/


#include <stdio.h>

// ����1�� 71�������ͬ��Ԫ��
int n1[9] = {34, 65, 109, 110, 109, 71, 110, 34, 65};

// ����2�� 71, 125 ����������ͬ��Ԫ��
int n2[10] = {34, 110, 65, 109, 109, 71, 110, 34, 125, 65};


void find1()
{
    int rs = 0;
    for(unsigned int i=0; i<sizeof(n1)/sizeof(int); i++)
    {
        rs = rs^n1[i]; // ����������ȫ����򣬸����������ԣ���ͬ���ֽ�����򣬽��Ϊ0��
    }
    printf("n1: %d\n", rs);
}

void find2()
{
    // step 1: ����������ȫ�����ͬfind1()���ó��Ľ��rsʵ������������Ԫ�ص��������
    int rs = 0;
    for(unsigned int i=0; i<sizeof(n2)/sizeof(int); i++)
    {
        rs = rs^n2[i]; 
    }
    
    // step 2: ������Ԫ��������1bit�ǲ�ͬ�ģ�����rs������1bit��1���ҳ�����λ��j��.
    int j=0;
    int tmp = 0x00;
    while(1)
    {
        tmp = rs&(0x01<<j);
        if(tmp > 0)
            break;
        j++;
    };
    
    // step 3: ���е�Ԫ���У�jλҪô��1��Ҫô��0���������������Ԫ�طֿ���
    // jλΪ1�Ľ������jλΪ0�Ľ�����������͵ó���������Ԫ���ˡ�
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
