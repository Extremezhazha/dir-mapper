//
// Created by zhazha on 11/16/25.
//

#ifndef DIR_MAPPER_CREATEDIR_ACTION_H
#define DIR_MAPPER_CREATEDIR_ACTION_H
#include <string>

#include "map_action.h"

namespace mapper {
    class createdir_action : public mapper_action {
        int apply(dir_tree::node & target_node) override;
        std::string explain() override;

        bool is_dir_structure() override;
    };
} // mapper

#endif //DIR_MAPPER_CREATEDIR_ACTION_H