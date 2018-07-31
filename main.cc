#include <iostream>
#include <algorithm>
#include "./utils/types/ComparsionResult.h"

struct Test {
    int val;

    Test(int val): val{val} { }

    Test(const Test& t): val{t.val} { }

    Test(Test&& t): val{t.val} { }

    operator int() const noexcept {
        return val;
    }
};

line::utils::types::ComparsionResult compare(const Test& l, const Test& r) {
    using line::utils::types::ComparsionResult;
    return l.val == r.val ? ComparsionResult::Equal : (l.val < r.val ? ComparsionResult::Less : ComparsionResult::Greater);
}

line::utils::types::ComparsionResult compare(const Test& l, int r) {
    using line::utils::types::ComparsionResult;
    return l.val == r ? ComparsionResult::Equal : (l.val < r ? ComparsionResult::Less : ComparsionResult::Greater);
}

#include "./dataStructures/BST/AVLTree.thd"

#include<queue>
template<typename Node>
void print(const Node* root) {
    if(root) {
        std::queue<const Node*> level;
        level.push(root);
        while(!level.empty()) {
            const std::size_t size = level.size();
            for(std::size_t c = 0; c < size; ++c) {
                const Node* node = level.front();
                level.pop();
                if(node->left) {
                    level.push(node->leftNode());
                }
                if(node->right) {
                    level.push(node->rightNode());
                }
                std::cout << node->data << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }
}

int main() {
    line::dataStructures::BST::AVLTree<Test, int> tree;


    for(int i = 1; i < 32; ++i) {
        tree.insert(Test{i});
        print(tree.root);
    }

    tree.insert(Test{5});
    {
        line::utils::types::Optional<Test&> result = tree.find(5);
        std::cout << "hasValue: " << result.hasValue() << std::endl;
        if(result.hasValue()) {
            std::cout << "value: " << result.value().val << std::endl;
        }
    }
    print(tree.root);

    {
        line::utils::types::Optional<Test&> result = tree.find(0);
        std::cout << "hasValue: " << result.hasValue() << std::endl;
        if(result.hasValue()) {
            std::cout << "value: " << result.value().val << std::endl;
        }
    }
    print(tree.root);
    
    return 0;
}
