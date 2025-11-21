#ifndef DIR_MAPPER_MAPPER_H
#define DIR_MAPPER_MAPPER_H
#include <memory>

#include "map_action.h"
#include "node.h"

namespace mapper {
    class mapper {
    public:
        virtual ~mapper() = default;

        using result_t = std::pair<bool, std::unique_ptr<mapper_action> >;

        virtual result_t apply(dir_tree::node &source_node, dir_tree::node *target_node) = 0;

        virtual bool should_try_apply(dir_tree::node &source_node) = 0;

        virtual std::string mapped_name(dir_tree::node &source_node) = 0;
    };
}

#endif //DIR_MAPPER_MAPPER_H
