//
// Created by zhazha on 11/16/25.
//

#ifndef DIR_MAPPER_MAP_ACTION_H
#define DIR_MAPPER_MAP_ACTION_H

namespace dir_tree {
    struct node;
}

namespace mapper {
    class mapper_action {
    public:
        virtual ~mapper_action() = default;

        virtual int apply(dir_tree::node & target_node) = 0;
        virtual std::string explain() = 0;
        virtual bool is_dir_structure() {
            return false;
        }
    };
}

#endif //DIR_MAPPER_MAP_ACTION_H