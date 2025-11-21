#ifndef DIR_MAPPER_TREE_H
#define DIR_MAPPER_TREE_H
#include "node.h"

namespace dir_tree {
    class tree {
    public:
        node root;

        explicit tree(const std::filesystem::path &root_path);

        void init();
    };
} // dir_tree

#endif //DIR_MAPPER_TREE_H
