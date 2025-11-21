#ifndef DIR_MAPPER_PASS_ORCHESTRATOR_H
#define DIR_MAPPER_PASS_ORCHESTRATOR_H
#include "pass.h"
#include "tree.h"

namespace mapper {
    class pass_orchestrator {
        dir_tree::node source_tree;
        dir_tree::node target_tree;
        std::vector<std::unique_ptr<pass> > passes;

    public:
        pass_orchestrator(
            std::filesystem::path const &source_path,
            std::filesystem::path const &target_path,
            std::vector<std::unique_ptr<pass> > passes
        );

        int run(bool debug);
    };
} // mapper

#endif //DIR_MAPPER_PASS_ORCHESTRATOR_H
