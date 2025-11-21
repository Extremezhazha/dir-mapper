#ifndef DIR_MAPPER_CLEANUP_UNAUDITED_PASS_H
#define DIR_MAPPER_CLEANUP_UNAUDITED_PASS_H
#include "pass.h"

namespace mapper::passes {
    class cleanup_unaudited_pass : public pass {
    public:
        int run(dir_tree::node &source, dir_tree::node &target, bool debug) override;
    };
}

#endif //DIR_MAPPER_CLEANUP_UNAUDITED_PASS_H
