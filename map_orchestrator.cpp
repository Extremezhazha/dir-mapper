//
// Created by zhazha on 11/16/25.
//

#include "map_orchestrator.h"

#include <iostream>

#include "thread_pool.h"
#include "utils.h"

namespace orchestration {
    void map_orchestrator::init_source_tree() {
        source_tree.init();

        if (enable_concurrent_processing) {
            execution::thread_pool<int> preprocessing_pool{thread_pool_size};
            for (auto & preprocessor: preprocessors) {
                source_tree.root.visit_all([&preprocessing_pool, &preprocessor](dir_tree::node & node) {
                    preprocessing_pool.submit([&node, &preprocessor] {
                        return preprocessor(node);
                    });
                }, false);
            }
        } else {
            for (auto & preprocessor: preprocessors) {
                source_tree.root.visit_all(preprocessor, false);
            }
        }
    }

    void map_orchestrator::setup_target_structure() {
        target_tree.root.visit_all([](dir_tree::node & node) {
            if (node.ctx.audited && node.ctx.action && node.ctx.action->is_dir_structure()) {
                node.ctx.action->apply(node);
            }
        }, false);
    }

    void map_orchestrator::run_target_actions() {
        if (enable_concurrent_processing) {
            execution::thread_pool<int> processing_pool{thread_pool_size};
            target_tree.root.visit_all([&processing_pool](dir_tree::node &node) {
                processing_pool.submit([&node] {
                    if (node.ctx.audited && node.ctx.action && !node.ctx.action->is_dir_structure()) {
                        node.ctx.action->apply(node);
                    }
                    return 0;
                });
            }, false);
        } else {
            target_tree.root.visit_all([](dir_tree::node &node) {
                if (node.ctx.audited && node.ctx.action && !node.ctx.action->is_dir_structure()) {
                    node.ctx.action->apply(node);
                }
            }, false);
        }
    }

    void map_orchestrator::create_mapping_rec(dir_tree::node & source_node, dir_tree::node & target_node) {
        for (auto & [source_next_name, source_next]: source_node.m_map) {
            dir_tree::node * target_next = nullptr;
            for (auto & mapper: mappers) {
                if (mapper && mapper->should_try_apply(source_next)) {
                    std::string target_next_name = mapper->mapped_name(source_next);

                    dir_tree::node * found_next = target_node.get_next(target_next_name);

                    auto [should_stop, action] = mapper->apply(source_next, found_next);
                    if (action) {
                        if (!found_next) {
                            std::filesystem::path target_next_path = target_node.path / target_next_name;
                            auto [it, _] = target_node.m_map.try_emplace(target_next_name, target_next_path, true);
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

    map_orchestrator::map_orchestrator(
        int thread_pool_size,
        bool enable_concurrent_processing,
        preprocessor_vec_t preprocessors,
        mapper_vec_t mappers,
        std::filesystem::path const &source_path,
        std::filesystem::path const &target_path):
    thread_pool_size{thread_pool_size},
    enable_concurrent_processing{enable_concurrent_processing},
    preprocessors{std::move(preprocessors)},
    mappers{std::move(mappers)},
    source_tree{source_path},
    target_tree{target_path}
    {}

    void map_orchestrator::init(bool print_debug) {
        init_source_tree();

        target_tree.init();
        if (print_debug) {
            std::cout << "=============== Source directory: ===============" << std::endl;
            source_tree.root.print();
            std::cout << "=============== Target directory: ===============" << std::endl;
            target_tree.root.print();
        }
    }

    void map_orchestrator::create_mapping(bool print_debug) {
        create_mapping_rec(source_tree.root, target_tree.root);
        if (print_debug) {
            target_tree.root.eval_recursive_print_tag("has_action", [](dir_tree::node & node) {
                return static_cast<bool>(node.ctx.action);
            });
            std::cout << "=============== Target actions: ===============" << std::endl;
            target_tree.root.print("has_action");
        }
    }

    void map_orchestrator::do_work() {
        setup_target_structure();
        run_target_actions();
    }
} // orchestration