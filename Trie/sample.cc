//#include "trie.hpp"
//
//#include <iostream>
//#include <string>
//#include <cassert>
//
//int main()
//{
//    trie<std::string> t;
//
//    t.insert("apple");
//    t.insert("banana");
//    t.insert("cherry");
//    t.insert("grape");
//    t.insert("orange");
//
//    // Check the size of the Trie
//    assert(t.size() == 5);
//
//    // Check the presence of strings
//    assert(t.has("pear") == false);
//    assert(t.has("banana") == true);
//
//    // Check autocompletion
//    auto candidates = t.complete("or");
//    assert(candidates.size() == 1);
//
//    candidates = t.complete("ap");
//    assert(candidates.size() == 1);
//
//    std::cout << "All tests passed." << std::endl;
//}
