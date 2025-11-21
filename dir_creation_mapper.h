#ifndef DIR_MAPPER_DIR_CREATION_MAPPER_H
#define DIR_MAPPER_DIR_CREATION_MAPPER_H
#include "mapper.h"

namespace mapper {
    class dir_creation_mapper : public mapper {
    public:
        ~dir_creation_mapper() override;

        result_t apply(dir_tree::node &source_node, dir_tree::node *target_node) override;

        bool should_try_apply(dir_tree::node &source_node) override;

        std::string mapped_name(dir_tree::node &source_node) override;
    };
} // mapper

#endif //DIR_MAPPER_DIR_CREATION_MAPPER_H
