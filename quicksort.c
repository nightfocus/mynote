/*
    快速排序C代码
    gcc -g -o quicksort quicksort.c
*/

#include <stdio.h> 

int a[10] = {6 , 1,  2, 7,  9,  13,  4,  5, 10,  8};

// left, right为待排数组的前后下标
void quicksort(int left,int right) 
{ 
    int i,j,t,temp; 
    if(left > right) 
       return; 
                                
    temp=a[left]; //temp中存的就是基准数 
    i=left; 
    j=right; 
    while(i!=j) 
    { 
        //顺序很重要，要先从右边开始找 
        while(a[j]>=temp && i<j) 
            j--; 
        //再找左边的 
        while(a[i]<=temp && i<j) 
            i++; 
        //交换两个数在数组中的位置 
        if(i<j) 
        { 
            t=a[i]; 
            a[i]=a[j]; 
            a[j]=t; 
        } 
    } 
    
    //最终将基准数归位
    a[left]=a[i]; 
    a[i]=temp; 
                             
    quicksort(left,i-1); //继续处理左边的，这里是一个递归的过程
    quicksort(i+1,right); //继续处理右边的 ，这里是一个递归的过程
}

int main() 
{ 
    int i; 
    int n = 9;
    quicksort(0, n); //快速排序调用, left, right为下标
                             
    //输出排序后的结果 
    for(i=1;i<=n;i++) 
        printf("%d ", a[i]);
    
    getchar();
    getchar(); 
    return 0; 
} 
