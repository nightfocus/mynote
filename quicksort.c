/*
    ��������C����
    gcc -g -o quicksort quicksort.c
*/

#include <stdio.h> 

int a[10] = {6 , 1,  2, 7,  9,  13,  4,  5, 10,  8};

// left, rightΪ���������ǰ���±�
void quicksort(int left,int right) 
{ 
    int i,j,t,temp; 
    if(left > right) 
       return; 
                                
    temp=a[left]; //temp�д�ľ��ǻ�׼�� 
    i=left; 
    j=right; 
    while(i!=j) 
    { 
        //˳�����Ҫ��Ҫ�ȴ��ұ߿�ʼ�� 
        while(a[j]>=temp && i<j) 
            j--; 
        //������ߵ� 
        while(a[i]<=temp && i<j) 
            i++; 
        //�����������������е�λ�� 
        if(i<j) 
        { 
            t=a[i]; 
            a[i]=a[j]; 
            a[j]=t; 
        } 
    } 
    
    //���ս���׼����λ
    a[left]=a[i]; 
    a[i]=temp; 
                             
    quicksort(left,i-1); //����������ߵģ�������һ���ݹ�Ĺ���
    quicksort(i+1,right); //���������ұߵ� ��������һ���ݹ�Ĺ���
}

int main() 
{ 
    int i; 
    int n = 9;
    quicksort(0, n); //�����������, left, rightΪ�±�
                             
    //��������Ľ�� 
    for(i=1;i<=n;i++) 
        printf("%d ", a[i]);
    
    getchar();
    getchar(); 
    return 0; 
} 
