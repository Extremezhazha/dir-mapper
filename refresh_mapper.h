#ifndef DIR_MAPPER_REFRESH_MAPPER_H
#define DIR_MAPPER_REFRESH_MAPPER_H
#include "mapper.h"

namespace mapper {
    class refresh_mapper : public mapper {
        std::unique_ptr<mapper> delegate;
        bool enforce_hash;

    public:
        ~refresh_mapper() override;

        refresh_mapper(std::unique_ptr<mapper> delegate, bool enforce_hash);

        result_t apply(dir_tree::node &source_node, dir_tree::node *target_node) override;

        bool should_try_apply(dir_tree::node &source_node) override;

        std::string mapped_name(dir_tree::node &source_node) override;
    };
} // mapper

#endif //DIR_MAPPER_REFRESH_MAPPER_H
