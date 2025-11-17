//
// Created by zhazha on 11/16/25.
//

#include "dir_creation_mapper.h"

#include "createdir_action.h"

namespace mapper {
    dir_creation_mapper::~dir_creation_mapper() = default;

    mapper::result_t dir_creation_mapper::apply(dir_tree::node &source_node, dir_tree::node *target_node) {
        if (target_node && target_node->exists && target_node->filetype == std::filesystem::file_type::directory) {
            return std::make_pair(true, nullptr);
        }
        return target_node ?
        std::make_pair(false, nullptr) :
        std::make_pair(true, std::make_unique<createdir_action>());
    }

    bool dir_creation_mapper::should_try_apply(dir_tree::node &source_node) {
        return source_node.filetype == std::filesystem::file_type::directory;
    }

    std::string dir_creation_mapper::mapped_name(dir_tree::node &source_node) {
        return source_node.path.filename();
    }
} // mapper