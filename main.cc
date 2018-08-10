/*#include <iostream>
#include "./core/DirectoryStructure.thd"
#include "./core/String/String.h"

int main() {
    line::core::DirectoryStructure<line::core::String> dirStruct{"~/tmp", "line"};

    {
        dirStruct.insert(
            line::core::FilePathIterator{line::core::String::StringSlice{"file.txt", 8}},
            line::core::String{"some-hash"}
        );
        line::utils::types::Optional<line::core::String&> result =  dirStruct.find(
            line::core::FilePathIterator{line::core::String::StringSlice{"file.txt", 8}});

        if(result) {
            std::cout << result.value().cString() << std::endl;
        } else {
            std::cout << "File not found" << std::endl;
        }
    }

    {
        dirStruct.insert(
            line::core::FilePathIterator{line::core::String::StringSlice{"a/f1.txt", 8}},
            line::core::String{"hash1"}
        );
        line::utils::types::Optional<line::core::String&> result =  dirStruct.find(
            line::core::FilePathIterator{line::core::String::StringSlice{"a/f1.txt", 8}});

        if(result) {
            std::cout << result.value().cString() << std::endl;
        } else {
            std::cout << "File not found" << std::endl;
        }
    }

    {
        dirStruct.insert(
            line::core::FilePathIterator{line::core::String::StringSlice{"a/b/f.txt", 9}},
            line::core::String{"hash"}
        );
        line::utils::types::Optional<line::core::String&> result =  dirStruct.find(
            line::core::FilePathIterator{line::core::String::StringSlice{"a/b/f.txt", 9}});

        if(result) {
            std::cout << result.value().cString() << std::endl;
        } else {
            std::cout << "File not found" << std::endl;
        }
    }

    {
        dirStruct.insert(
            line::core::FilePathIterator{line::core::String::StringSlice{"a/f2.txt", 8}},
            line::core::String{"hash2"}
        );
        line::utils::types::Optional<line::core::String&> result =  dirStruct.find(
            line::core::FilePathIterator{line::core::String::StringSlice{"a/f3.txt", 8}});

        if(result) {
            std::cout << result.value().cString() << std::endl;
        } else {
            std::cout << "File not found" << std::endl;
        }
    }
    
    return 0;
}*/

#include <iostream>
#include "./dataStructures/BST/AVLTree.thd"
#include "./utils/funcs/integerToComparsionResult.thd"

line::utils::types::ComparsionResult compare(const int&a, const int& b) {
    return line::utils::funcs::integerToComparsionResult(a - b);
}

int main() {
    line::dataStructures::BST::AVLTree<int, int, compare, compare> tree;

    for(int i = 1; i < 32; ++i) {
        tree.insert(i);
    }

    line::dataStructures::BST::AVLTree<int, int, compare, compare>::ConstIterator iter = tree.constIterator();

    while(iter) {
        std::cout << *iter << std::endl;
        ++iter;
    }

    line::dataStructures::BST::AVLTree<int, int, compare, compare>::Iterator iterator = tree.iterator();

    while(iterator) {
        ++(*iterator);
        ++iterator;
    }

    iter = tree.constIterator();

    while(iter) {
        std::cout << *iter << std::endl;
        ++iter;
    }

    return 0;
}