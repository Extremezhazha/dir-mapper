#ifndef DIR_MAPPER_HASH_GENERATING_PASS_H
#define DIR_MAPPER_HASH_GENERATING_PASS_H
#include "pass.h"

namespace mapper::passes {
    class hash_generating_pass : public pass {
    public:
        hash_generating_pass(bool enable_parallel, int thread_pool_size);

    private:
        bool enable_parallel;
        int thread_pool_size;

    public:
        int run(dir_tree::node &source, dir_tree::node &target, bool debug) override;
    };
}

#endif //DIR_MAPPER_HASH_GENERATING_PASS_H
