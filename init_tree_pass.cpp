#include "init_tree_pass.h"

#include <iostream>

namespace mapper::passes {
    void init_tree_pass::scan_node_recursive(dir_tree::node &current_node) {
        if (auto const &base_path = current_node.path; std::filesystem::is_directory(base_path)) {
            for (auto const &entry: std::filesystem::directory_iterator(
                     base_path, std::filesystem::directory_options::skip_permission_denied)) {
                auto const &next_path = entry.path();
                auto const &filename_segment = next_path.filename();
                std::string filename_str = filename_segment.string();
                if (const dir_tree::node *found_next_node = current_node.get_next(filename_str); !found_next_node) {
                    auto [it, _] = current_node.m_map.try_emplace(filename_str, next_path);
                    scan_node_recursive(it->second);
                }
            }
        }
    }

    int init_tree_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        scan_node_recursive(source);
        scan_node_recursive(target);
        target.ctx.audited = true;
        if (debug) {
            std::cout << "=================== Init Source tree ===================" << std::endl;
            source.print();
            std::cout << "=================== Init Target tree ===================" << std::endl;
            target.print();
        }
        return 0;
    }
}
