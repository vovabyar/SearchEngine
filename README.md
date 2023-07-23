# Trie-Based Search Engine

## Overview
This project utilizes the power of the Trie data structure to provide efficient and lightning-fast search capabilities.

## Features
- Trie Elegance: My implementation leverages the elegance and efficiency of the Trie data structure to facilitate blazingly fast searches.
- Simplicity and Portability: It's designed to be lightweight, simple to understand, and highly portable, ensuring you can use it across various platforms with ease.

## Why Trie?
Trie is an excellent choice for autocomplete and search functionality due to its ability to store and retrieve data with incredible speed. Whether you're building a search engine for text, numbers, or any other data type, Trie offers a compact and efficient solution.

## API Trie
- construct trie<K> or trie<K,V> as desired.
- .insert(element) or [element] to create/access elements.
- .has(element) returns true if element is in trie.
- .complete(element) autocompletes partially provided element then returns vector of candidate pointers.
- .list() returns a vector of hosted pointers in trie.
- .size() returns number of elements in trie.

## Complexities
- Insertion (Insert): O(m) - m is the length of the string. This operation takes time proportional to the length of the inserted string.
- Deletion (Delete): O(m) - m is the length of the string. Similar to insertion, the deletion operation also takes time proportional to the length of the deleted string.
- Search (Search): O(m) - m is the length of the string. Just like insertion and deletion, the search operation requires time proportional to the length of the searched string.
- Space Complexity: O(n*m) - n is the number of strings, m is the average length of strings. This complexity arises because for each letter in each inserted string, a Trie node is created. As the number of strings and their lengths increase, the space complexity also increases.

## Sample
```
c++
#include "trie.hpp"

#include <iostream>
#include <string>
#include <cassert>

int main()
{
    trie<std::string> t;

    t.insert("apple");
    t.insert("banana");
    t.insert("cherry");
    t.insert("grape");
    t.insert("orange");

    // Check the size of the Trie
    assert(t.size() == 5);

    // Check the presence of strings
    assert(t.has("pear") == false);
    assert(t.has("banana") == true);

    // Check autocompletion
    auto candidates = t.complete("or");
    assert(candidates.size() == 1);

    candidates = t.complete("ap");
    assert(candidates.size() == 1);
    
    std::cout << "All tests passed." << std::endl;
}
```
## Video Search Integration
I've integrated my Trie-based Search Engine with a JSON file, videos.json, which contains information about videos. 
Video Manager Class

The VideosManager class handles the integration of video data and search functionality. It includes methods to search for similar video titles using Trie and return the results in JSON format.
### Sample Usage
```
c++
int main() {
    VideosManager manager;
    manager.loadFromJson("videos.json");

    // Populate the Trie with video titles
    manager.populateTrie();

    std::string searchQuery = "The A";
    json searchResults = manager.searchSimilarTitles(searchQuery);

    std::cout << "Search results for '" << searchQuery << "':" << std::endl;
    std::cout << searchResults.dump(4) << std::endl; // Print the JSON results
    /*
        Search results for 'The A':
        [
            {
                "author": "Billy Wilder",
                "duration": 7500,
                "id": 90,
                "title": "The Apartment (1960)",
                "views": 11151432
            }
        ]
    */
    return 0;
}
```
## TODO: 


1. Responsive Web Interface: Develop a responsive and user-friendly web interface for the search engine, optimizing it for both desktop and mobile devices.

5. Search Suggestions and Auto-Completion: Implement real-time search suggestions and auto-completion to assist users in refining their search queries more efficiently.
1. Enhanced Search Functionality: Implement advanced search features, such as filtering, and sorting, to provide users with more comprehensive search options.

2. Integration with External Data Sources: Extend the search engine's capabilities by integrating with external data sources, such as APIs or databases, to enrich search results with additional information.