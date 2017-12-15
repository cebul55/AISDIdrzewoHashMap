#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{

template <typename K, typename V>
using Tree = aisdi::TreeMap<K, V>;

template <typename K, typename V>
using Hash = aisdi::HashMap<K, V>;


void performAddingTest(){
    Hash<int,int> hash;
    Tree<int,int> tree;
    auto start = std::chrono::system_clock::now();
    for(int i = 0 ; i < 700000 ; ++i){
        tree.valueOf(i) = i;
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsedSecondsTree = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished adding to tree at "<< std::ctime(&end_time)
              << "elapsed time: "<<elapsedSecondsTree.count() <<"s\n";

    start = std::chrono::system_clock::now();
    for(int i = 0 ; i < 700000 ; ++i){
        hash.valueOf(i) = i;
    }
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsedSecondsHash = end - start;
    std::time_t end_timeH = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished adding to hash at "<< std::ctime(&end_timeH)
              << "elapsed time: "<<elapsedSecondsHash.count() <<"s\n";
//iterating
    start = std::chrono::system_clock::now();
    auto it5 = tree.begin();
    std::cout<<it5->second;
    auto it2 = tree.end();
    while(it5 == it2){++it5;}
    end = std::chrono::system_clock::now();

    elapsedSecondsTree = end - start;
    end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished iterating tree at "<< std::ctime(&end_time)
              << "elapsed time: "<<elapsedSecondsTree.count() <<"s\n";

    start = std::chrono::system_clock::now();
    auto it3 = hash.begin();
    auto it4 = hash.end();
    --it4;
    while(it3 == it4){++it3;}
    end = std::chrono::system_clock::now();

    elapsedSecondsHash = end - start;
    end_timeH = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished iterating hash at "<< std::ctime(&end_timeH)
              << "elapsed time: "<<elapsedSecondsHash.count() <<"s\n";
//deleting
         start = std::chrono::system_clock::now();
        tree.clean();
         end = std::chrono::system_clock::now();

        elapsedSecondsTree = end - start;
        end_time = std::chrono::system_clock::to_time_t(end);

        std::cout << "finished deleting tree at "<< std::ctime(&end_time)
                  << "elapsed time: "<<elapsedSecondsTree.count() <<"s\n";

        start = std::chrono::system_clock::now();
        hash._clear();
        end = std::chrono::system_clock::now();

        elapsedSecondsHash = end - start;
        end_timeH = std::chrono::system_clock::to_time_t(end);

        std::cout << "finished deleting hash at "<< std::ctime(&end_timeH)
                  << "elapsed time: "<<elapsedSecondsHash.count() <<"s\n";

    }

} // namespace

int main(){
    performAddingTest();
}