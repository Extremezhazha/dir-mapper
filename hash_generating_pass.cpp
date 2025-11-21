
#include <iostream>
#include <regex>

#include "hash_generating_pass.h"
#include "thread_pool.h"
#include "utils.h"

namespace {
    void probe_m4a_file(dir_tree::node &node) {
        if (node.exists && node.filetype == std::filesystem::file_type::regular) {
            std::ostringstream cmd;
            cmd << "md5sum " << node.path;
            std::string result = execution::run_cmd(cmd.str());

            const std::regex hash_regex{R"(^([^ ]+))"};

            std::smatch match;
            if (std::regex_search(result, match, hash_regex)) {
                node.ctx.hash = match[1];
            }
        }
    }
}

namespace mapper::passes {
    hash_generating_pass::hash_generating_pass(bool enable_parallel, int thread_pool_size) : enable_parallel{
            enable_parallel
        },
        thread_pool_size{thread_pool_size} {
    }

    int hash_generating_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        std::vector trees{std::ref(source), std::ref(target)};
        if (enable_parallel) {
            execution::thread_pool<int> preprocessing_pool{thread_pool_size};
            for (auto &current: trees) {
                current.get().visit_all([&preprocessing_pool](dir_tree::node &node) {
                    preprocessing_pool.submit([&node] {
                        probe_m4a_file(node);
                        return 0;
                    });
                }, false);
            }
        } else {
            for (auto &current: trees) {
                current.get().visit_all(probe_m4a_file, false);
            }
        }
        if (debug) {
            std::cout << "=================== Hashed trees ===================" << std::endl;
            source.print();
            target.print();
        }
        return 0;
    }
}
