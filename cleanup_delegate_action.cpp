//
// Created by zhazha on 11/16/25.
//

#include "cleanup_delegate_action.h"

#include <filesystem>

#include "node.h"

namespace mapper {
    cleanup_delegate_action::cleanup_delegate_action(std::unique_ptr<mapper_action> delegate):
    delegate{std::move(delegate)} {}

    int cleanup_delegate_action::apply(dir_tree::node &target_node) {
        if (!std::filesystem::remove(target_node.path))
            return 1;
        return delegate->apply(target_node);
    }

    std::string cleanup_delegate_action::explain() {
        return "<Clean up and apply next> " + delegate->explain();
    }
}
