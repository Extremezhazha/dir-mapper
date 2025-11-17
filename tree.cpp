//
// Created by zhazha on 11/15/25.
//

#include "tree.h"
#include <iostream>

namespace dir_tree {
    tree::tree(const std::filesystem::path &root_path): root{root_path} {}

    void scan_node_recursive(node & current_node) {
        if (auto const & base_path = current_node.path; std::filesystem::is_directory(base_path)) {
            for (auto const & entry : std::filesystem::directory_iterator(
                base_path, std::filesystem::directory_options::skip_permission_denied)) {
                auto const & next_path = entry.path();
                auto const & filename_segment = next_path.filename();
                std::string filename_str = filename_segment.string();
                if (const node * found_next_node = current_node.get_next(filename_str); !found_next_node) {
                    auto [it, _] = current_node.m_map.try_emplace(filename_str, next_path);
                    scan_node_recursive(it->second);
                }
            }
        }
    }

    void tree::init() {
        scan_node_recursive(root);
    }
} // dir_tree