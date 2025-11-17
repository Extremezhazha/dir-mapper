//
// Created by zhazha on 11/16/25.
//

#ifndef DIR_MAPPER_FLACCONVERSION_ACTION_H
#define DIR_MAPPER_FLACCONVERSION_ACTION_H
#include <string>

#include "map_action.h"

namespace mapper {
    class flacconversion_action: public mapper_action {
        dir_tree::node & source_node;
    public:
        explicit flacconversion_action(dir_tree::node &source_node);

        int apply(dir_tree::node & target_node) override;
        std::string explain() override;
    };
} // mapper

#endif //DIR_MAPPER_FLACCONVERSION_ACTION_H