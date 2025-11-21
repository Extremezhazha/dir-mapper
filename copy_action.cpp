#include "copy_action.h"

#include <format>

#include "node.h"

namespace mapper {
    copy_action::copy_action(dir_tree::node &source_node) : source_node{source_node} {
    }

    int copy_action::apply(dir_tree::node &target_node) {
        if (!std::filesystem::copy_file(source_node.path, target_node.path))
            return 1;
        std::filesystem::permissions(target_node.path,
                                     std::filesystem::perms::owner_read | std::filesystem::perms::owner_write |
                                     std::filesystem::perms::group_read | std::filesystem::perms::group_write |
                                     std::filesystem::perms::others_read,
                                     std::filesystem::perm_options::replace);
        return 0;
    }

    std::string copy_action::explain() {
        std::ostringstream ostringstream;
        ostringstream << "<Copying from " << source_node.path << ">";
        return ostringstream.str();
    }
} // mapper
