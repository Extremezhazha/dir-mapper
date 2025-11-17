//
// Created by zhazha on 11/16/25.
//

#ifndef DIR_MAPPER_CLEANUP_DELEGATE_ACTION_H
#define DIR_MAPPER_CLEANUP_DELEGATE_ACTION_H

#include <memory>
#include <string>

#include "map_action.h"

namespace mapper {
    class cleanup_delegate_action : public mapper_action {
        std::unique_ptr<mapper_action> delegate;
    public:
        explicit cleanup_delegate_action(std::unique_ptr<mapper_action> delegate);

        int apply(dir_tree::node & target_node) override;
        std::string explain() override;
    };
} // mapper


#endif //DIR_MAPPER_CLEANUP_DELEGATE_ACTION_H