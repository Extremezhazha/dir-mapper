//
// Created by zhazha on 11/16/25.
//

#include "noop_action.h"

namespace mapper {
    int noop_action::apply(dir_tree::node &target_node) {
        return 0;
    }

    std::string noop_action::explain() {
        return "<No action to take>";
    }
} // mapper