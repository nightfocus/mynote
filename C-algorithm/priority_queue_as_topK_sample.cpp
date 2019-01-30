// priority_queue::empty
#include <iostream>       // std::cout
#include <queue>          // std::priority_queue
#include <vector>

using namespace std;

int main ()
{
  std::priority_queue<int, vector<int>, greater<int> > mypq;
  int ais[] = {3,5,2,33,90, 123, 4208,28,330};

  for (int i=0; i<sizeof(ais)/sizeof(int); i++)
  {
      if(mypq.size() == 5)
      {
          mypq.pop();
      }
      mypq.push(ais[i]);
  }

  while (!mypq.empty())
  {
     int tmp = mypq.top();
     mypq.pop();
     cout << tmp << endl;
  }


  return 0;
}
