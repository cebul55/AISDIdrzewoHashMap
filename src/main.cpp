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
/*    //Map<int,int> *map = new Map<int,int>;
    Map<int,int> map;
    Map<int,int> other = { { 42, 5 }, { 27, 20 } };

    map = other;
    std::cout<<other.isEmpty()<<std::endl;*/
    Map<int,int> map = { { 42, 5 }, { 27, 20 } };
//    Map<int,int> map1;
    Map<int,int> map2{std::move(map)};
    //map = map2;
    //map.remove(27);
    //map._clear(map._root);
//    std::cout<<map.begin()->second;

    std::cout<<map2.isEmpty();
}