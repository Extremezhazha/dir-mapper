//
// Created by zhazha on 11/16/25.
//

#ifndef DIR_MAPPER_MAP_ORCHESTRATOR_H
#define DIR_MAPPER_MAP_ORCHESTRATOR_H
#include "mapper.h"
#include "tree.h"

namespace orchestration {
    class map_orchestrator {
    public:
        using preprocessor_t = std::function<int(dir_tree::node&)>;
        using preprocessor_vec_t = std::vector<preprocessor_t>;
        using mapper_vec_t = std::vector<std::unique_ptr<mapper::mapper>>;
    private:
        int thread_pool_size;
        bool enable_concurrent_processing;
        preprocessor_vec_t preprocessors;
        mapper_vec_t mappers;
        dir_tree::tree source_tree;
        dir_tree::tree target_tree;

        void init_source_tree();
        void setup_target_structure();
        void run_target_actions();
        void create_mapping_rec(dir_tree::node & source_node, dir_tree::node & target_node);

    public:
        map_orchestrator(
            int thread_pool_size,
            bool enable_concurrent_processing,
            preprocessor_vec_t preprocessors,
            mapper_vec_t mappers,
            std::filesystem::path const & source_path,
            std::filesystem::path const & target_path
        );
        void init(bool print_debug);
        void create_mapping(bool print_debug);
        void do_work();
    };
} // orchestration

#endif //DIR_MAPPER_MAP_ORCHESTRATOR_H