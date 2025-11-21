#include "pass_orchestrator.h"

namespace mapper {
    pass_orchestrator::pass_orchestrator(
        std::filesystem::path const &source_path,
        std::filesystem::path const &target_path,
        std::vector<std::unique_ptr<pass> > passes) : source_tree{source_path},
                                                      target_tree{target_path},
                                                      passes{std::move(passes)} {
    }

    int pass_orchestrator::run(bool debug) {
        for (auto &pass: passes) {
            if (pass) {
                if (int result = pass->run(source_tree, target_tree, debug)) {
                    return result;
                }
            }
        }
        return 0;
    }
} // mapper
