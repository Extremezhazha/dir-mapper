//
// Created by zhazha on 11/16/25.
//

#include "createdir_action.h"

#include <filesystem>

#include "node.h"

namespace mapper {
    int createdir_action::apply(dir_tree::node &target_node) {
        std::filesystem::create_directory(target_node.path);
        return 0;
    }

    std::string createdir_action::explain() {
        return "<Dir to be created>";
    }

    bool createdir_action::is_dir_structure() {
        return true;
    }
} // mapper