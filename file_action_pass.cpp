#include <iostream>

#include "file_action_pass.h"
#include "thread_pool.h"

namespace mapper::passes {
    file_action_pass::file_action_pass(bool enable_parallel, int thread_pool_size) : enable_parallel{
            enable_parallel
        },
        thread_pool_size{thread_pool_size} {
    }

    int file_action_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        if (enable_parallel) {
            execution::thread_pool<int> preprocessing_pool{thread_pool_size};
            target.visit_all([&preprocessing_pool](dir_tree::node &node) {
                preprocessing_pool.submit([&node] {
                    if (node.filetype != std::filesystem::file_type::directory && node.ctx.action) {
                        node.ctx.action->apply(node);
                        node.probe();
                    }
                    return 0;
                });
            }, false);
        } else {
            target.visit_all([](dir_tree::node &node) {
                if (node.filetype != std::filesystem::file_type::directory && node.ctx.action) {
                    node.ctx.action->apply(node);
                    node.probe();
                }
            }, false);
        }
        if (debug) {
            std::cout << "=================== Acted output tree ===================" << std::endl;
            target.print();
        }
        return 0;
    }
}
