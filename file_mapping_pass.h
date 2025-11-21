#ifndef DIR_MAPPER_FILE_MAPPING_PASS_H
#define DIR_MAPPER_FILE_MAPPING_PASS_H
#include "mapper.h"
#include "pass.h"

namespace mapper::passes {
    class file_mapping_pass : public pass {
    public:
        using mapper_vec_t = std::vector<std::unique_ptr<mapper> >;

        file_mapping_pass(bool enable_parallel, int thread_pool_size, mapper_vec_t mappers);

    private:
        mapper_vec_t mappers;
        bool enable_parallel;
        int thread_pool_size;

        void create_mapping_rec(dir_tree::node &source, dir_tree::node &target);

    public:
        int run(dir_tree::node &source, dir_tree::node &target, bool debug) override;
    };
}

#endif //DIR_MAPPER_FILE_MAPPING_PASS_H
