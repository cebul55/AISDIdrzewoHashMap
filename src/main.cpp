#include <cstddef>
#include <cstdlib>
#include <string>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{

template <typename K, typename V>
using Map = aisdi::TreeMap<K, V>;

/*
void perfomTest()
{
  Map<int, std::string> map;
  map[1] = "TODO";
}
*/
    //TODO test line 384 385
} // namespace
/*
int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
  return 0;
}
*/
int main(){
  Map<int,int> l;
  l[10];
    int a = 2;
    char b;
    b = 'A'+((a-- != 1)? 0 : 1);
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<(7<<2)<<std::endl;

/*
    if(l.begin() != l.end())
    std::cout<<"true"<<std::endl;
  else
    std::cout<<"false"<<std::endl;
  std::cout<<l.begin()->first<<std::endl;
  std::cout<<l.end()->first<<std::endl;
*/
}