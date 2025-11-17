//
// Created by zhazha on 11/16/25.
//

#include "file_copy_mapper.h"

#include "copy_action.h"

namespace mapper {
    file_copy_mapper::~file_copy_mapper() = default;

    mapper::result_t file_copy_mapper::apply(dir_tree::node &source_node, dir_tree::node *target_node) {
        return std::make_pair(true, std::make_unique<copy_action>(source_node));
    }

    bool file_copy_mapper::should_try_apply(dir_tree::node &source_node) {
        return source_node.filetype == std::filesystem::file_type::regular;
    }

    std::string file_copy_mapper::mapped_name(dir_tree::node &source_node) {
        return source_node.path.filename();
    }
} // mapper