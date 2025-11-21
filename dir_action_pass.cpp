#include "dir_action_pass.h"

namespace mapper::passes {
    int dir_action_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        target.visit_all([](dir_tree::node &node) {
            if (!node.exists && node.ctx.audited && node.ctx.action && node.ctx.action->is_dir_structure()) {
                node.ctx.action->apply(node);
                node.probe();
            }
        }, false);
        return 0;
    }
}
