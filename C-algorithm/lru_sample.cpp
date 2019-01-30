/*
    用stl的map和list实现LRU算法
    map存储key和对应的value在链表中的位置
    总是将最近一次访问的K/V对象，存储在链表末端，所以每次get和set时，都要将原节点从链表中移除，再加入到链表末端。
*/
#include <iostream>
#include <map>
#include <list>
using namespace std;
 
struct CacheKVNode
{
    int key;
    string value;
    
    CacheKVNode(){}
    CacheKVNode(const CacheKVNode& obj)
    {
        cout << "call copy constructor" << endl;
        this->key = obj.key;
        this->value = obj.value;
    }
};

typedef map<int, list<CacheKVNode>::iterator> CacheMap;

class LRUCache
{
private:
    int maxCacheSize;
    list<CacheKVNode> cacheLst;
    CacheMap cMap;
    
public:
    LRUCache(int size) : maxCacheSize(size) 
    {
    }
    
    string get(int key) 
    {
        CacheMap::iterator it = cMap.find(key);
        if(it != cMap.end()) // key exists
        {
            list<CacheKVNode>::iterator listIt = cMap[key];
            
            cacheLst.push_front(*listIt); // push this node to the front of list.
            cMap[key] = cacheLst.begin();
            
            cacheLst.erase(listIt); // remove original node from list.
            return cacheLst.begin()->value;
        }
        
        return "not exists.";
    }
     
    void set(int key, const string& value) 
    {
        CacheMap::iterator it = cMap.find(key);
        if(it == cMap.end()) // key don't exist.
        {
            if(cacheLst.size() == maxCacheSize)  // cache is full.
            {
                cMap.erase(cacheLst.back().key);
                cacheLst.pop_back();
            }
            CacheKVNode newNode;
            newNode.key = key;
            newNode.value = value;
            cacheLst.push_front(newNode);
            cMap[key] = cacheLst.begin();
        }
        else  // key exists.
        {
            list<CacheKVNode>::iterator listIt = cMap[key];
            listIt->value = value;
            cacheLst.push_front(*listIt);
            cMap[key] = cacheLst.begin();
            
            cacheLst.erase(listIt);
        }
    }
};
 
 
int main(void)
{
    LRUCache cache(4);
    cache.set(1, "a");
    cache.set(2, "b");
    cache.set(3, "c");
    cache.set(4, "d");
    cout<<cache.get(1)<<endl;
    
    cache.set(4, "dd");
    cout<<cache.get(4)<<endl;
    cout<<cache.get(3)<<endl;
    cout<<cache.get(2)<<endl;
    cache.set(1, "aa");
    cache.set(6, "ff");
    cout<<cache.get(1)<<endl;
     
    cache.set(5, "e");
    cache.set(4, "dd");
     
    cout<<cache.get(1)<<endl;
    cout<<cache.get(2)<<endl;
    cout<<cache.get(3)<<endl;
    cout<<cache.get(4)<<endl;
    cout<<cache.get(5)<<endl;
    cout<<cache.get(6)<<endl;
     
    return 0;
}
