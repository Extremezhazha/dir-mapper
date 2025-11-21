#include "file_mapping_pass.h"

#include <iostream>


namespace mapper::passes {
    file_mapping_pass::file_mapping_pass(bool enable_parallel, int thread_pool_size,
                                         mapper_vec_t mappers) : enable_parallel{enable_parallel},
                                                                 thread_pool_size{thread_pool_size},
                                                                 mappers{std::move(mappers)} {
    }

    void file_mapping_pass::create_mapping_rec(dir_tree::node &source, dir_tree::node &target) {
        for (auto &[source_next_name, source_next]: source.m_map) {
            dir_tree::node *target_next = nullptr;
            for (auto &mapper: mappers) {
                if (mapper && mapper->should_try_apply(source_next)) {
                    std::string target_next_name = mapper->mapped_name(source_next);

                    dir_tree::node *found_next = target.get_next(target_next_name);

                    auto [should_stop, action] = mapper->apply(source_next, found_next);
                    if (action) {
                        if (!found_next) {
                            std::filesystem::path target_next_path = target.path / target_next_name;
                            auto [it, _] = target.m_map.try_emplace(target_next_name, target_next_path, true);
                            found_next = &it->second;
                        }
                        found_next->ctx.action = std::move(action);
                    }
                    found_next->ctx.audited = true;
                    if (should_stop) {
                        target_next = found_next;
                        break;
                    }
                }
            }
            if (target_next)
                create_mapping_rec(source_next, *target_next);
        }
    }

    int file_mapping_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        create_mapping_rec(source, target);
        if (debug) {
            std::cout << "=============== Target structure: ===============" << std::endl;
            target.print();
            target.eval_recursive_print_tag("has_action", [](dir_tree::node &node) {
                return static_cast<bool>(node.ctx.action);
            });
            std::cout << "=============== Target actions: ===============" << std::endl;
            target.print("has_action");
        }
        return 0;
    }
}
