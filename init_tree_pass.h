#ifndef DIR_MAPPER_INIT_TREE_PASS_H
#define DIR_MAPPER_INIT_TREE_PASS_H
#include "pass.h"

namespace mapper::passes {
    class init_tree_pass : public pass {
        static void scan_node_recursive(dir_tree::node &current_node);

    public:
        int run(dir_tree::node &source, dir_tree::node &target, bool debug) override;
    };
}

#endif //DIR_MAPPER_INIT_TREE_PASS_H
