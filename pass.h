#ifndef DIR_MAPPER_PASS_H
#define DIR_MAPPER_PASS_H
#include "node.h"

namespace mapper {
    class pass {
    public:
        virtual int run(dir_tree::node &source, dir_tree::node &target, bool debug) = 0;
    };
} // mapper

#endif //DIR_MAPPER_PASS_H
