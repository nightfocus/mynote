/*
    用stl的map和list实现LFU算法
*/
#include <iostream>
#include <map>
#include <list>
using namespace std;

struct CacheKVNode
{
    int key;
    string value;
    int count;
    
    CacheKVNode()
    {
        count = -1;
    }
    
    CacheKVNode(const CacheKVNode& obj)
    {
        cout << "call copy constructor" << endl;
        this->count = obj.count;
        this->key = obj.key;
        this->value = obj.value;
    }
};


class LFUCache {
private:
    int maxCacheSize;
    // 所有K/V中,访问频次最小值，最小为1
    int minFreq;
    // map的键即用户的key,值为value以及访问频次的关联map
    map<int, CacheKVNode> cacheMap;
    // map的键是访问频次，值为是在该频次下，所有key的链表
    map<int, list<int>> freqMap;
    // 键就是用户key, 值为freqMap中，key所在链表的位置，
    // 这个map的存在是为了解决从freqMap中找到对应的key，需要遍历list的情况
    map<int, list<int>::iterator> iterMap;
    
public:
    LFUCache(int size) 
    {
        maxCacheSize = size;
        minFreq = 0;
    }
    
    string get(int key) 
    {
        map<int, CacheKVNode>::iterator it = cacheMap.find(key);
        if (it == cacheMap.end()) 
            return "";
        
        freqMap[it->second.count].erase(iterMap[key]);
        it->second.count++;
        
        freqMap[it->second.count].push_back(key);
        iterMap[key] = --freqMap[it->second.count].end();
        
        if (freqMap[minFreq].size() == 0)
            minFreq++;
        
        return it->second.value;
    }
    
    void set(int key, const string& value) 
    {
        if (maxCacheSize <= 0)
            return;
        
        if (get(key) != "") 
        {
            cacheMap[key].value = value;
            return;
        }
        
        if (cacheMap.size() >= maxCacheSize)
        {
            cacheMap.erase(freqMap[minFreq].front());
            iterMap.erase(freqMap[minFreq].front());
            freqMap[minFreq].pop_front();
        }
        
        CacheKVNode node;
        node.key = key;
        node.value = value;
        node.count = 1;
        cacheMap[key] = node;
        
        freqMap[1].push_back(key);
        iterMap[key] = --freqMap[1].end();
        minFreq = 1;
    }
};

int main()
{
    LFUCache cache(3);
    
    cache.set(4, "444");
    cache.set(5, "555");
    cache.set(6, "666");
    cache.set(4, "444");
    cache.set(4, "444");
    cache.set(7, "777");
    cache.get(6);
    cache.get(6);
    cache.get(6);
    cache.get(6);
    cache.get(7);
    cache.get(7);
    cache.set(8, "888");
    cache.set(5, "555");
    
    cout << cache.get(4) << endl;
    cout << cache.get(5) << endl;
    cout << cache.get(6) << endl;
    cout << cache.get(7) << endl;
    cout << cache.get(8) << endl;
    
    
}

