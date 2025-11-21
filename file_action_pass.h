#ifndef DIR_MAPPER_FILE_ACTION_PASS_H
#define DIR_MAPPER_FILE_ACTION_PASS_H
#include "pass.h"

namespace mapper::passes {
    class file_action_pass : public pass {
    public:
        file_action_pass(bool enable_parallel, int thread_pool_size);

    private:
        bool enable_parallel;
        int thread_pool_size;

    public:
        int run(dir_tree::node &source, dir_tree::node &target, bool debug) override;
    };
}

#endif //DIR_MAPPER_FILE_ACTION_PASS_H
