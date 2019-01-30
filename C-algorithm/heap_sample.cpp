/*
    大顶堆实现
*/
#include <iostream>
#include <map>
#include <vector>
using namespace std;



void adjust(vector<int> &arr, int index, int len) 
{
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int max_index = index;
    if (left < len && arr[left] > arr[max_index]) 
        max_index = left;
    if (right < len && arr[right] > arr[max_index]) 
        max_index = right;
    
    if (max_index != index) 
    {
        swap(arr[max_index], arr[index]);
        adjust(arr, max_index, len); // 继续调整子节点
    }
}

void heapSort(vector<int> &arr, int len) 
{
    // 将数组处理成一个大顶堆
    for (int i = len / 2 - 1; i >= 0; i--) 
    {
        adjust(arr, i, len);
    }
   
    return;
   
}


int main()
{
    vector<int> abc = {2,3,6,1,9,0,4,7,5,8};
    int len = abc.size();
    
    heapSort(abc, len);

    for(auto i=abc.begin(); i!=abc.end(); i++)
    {
        cout << *i << endl;
    }
    
    cout << "---------------------------------" << endl;
    
    abc[0] = -1; // 模拟移除堆项的最大值，然后插入一个新的元素
    heapSort(abc, abc.size()); // 重新处理成一个大顶堆

    for(auto i=abc.begin(); i!=abc.end(); i++)
    {
        cout << *i << endl;
    }
    
    cout << "--------------- sorted ------------------" << endl;
    // 将每次堆排序得到的最大元素与当前规模的数组最后一个元素交换
    // 这会得到一个排序好的小顶堆
    for (int i = len - 1; i >= 1; i--) 
    {
        swap(abc[0], abc[i]);
        adjust(abc, 0, i);
    }
    for(auto i=abc.begin(); i!=abc.end(); i++)
    {
        cout << *i << endl;
    }
    
}

