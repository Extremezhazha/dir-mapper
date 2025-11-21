#include "refresh_mapper.h"

#include "cleanup_delegate_action.h"

namespace mapper {
    refresh_mapper::~refresh_mapper() = default;

    refresh_mapper::refresh_mapper(std::unique_ptr<mapper> delegate, bool enforce_hash) : delegate{std::move(delegate)},
        enforce_hash{enforce_hash} {
    }

    mapper::result_t refresh_mapper::apply(dir_tree::node &source_node, dir_tree::node *target_node) {
        result_t deletage_result = delegate->apply(source_node, target_node);
        if (!target_node || !target_node->exists) {
            return deletage_result;
        }
        if (target_node->exists && target_node->m_time > source_node.m_time) {
            if (enforce_hash) {
                if (!source_node.ctx.hash.empty() && source_node.ctx.hash == target_node->ctx.hash)
                    return std::make_pair(true, nullptr);
            } else
                return std::make_pair(true, nullptr);
        }
        return std::make_pair(deletage_result.first,
                              std::make_unique<cleanup_delegate_action>(std::move(deletage_result.second)));
    }

    bool refresh_mapper::should_try_apply(dir_tree::node &source_node) {
        return delegate->should_try_apply(source_node);
    }

    std::string refresh_mapper::mapped_name(dir_tree::node &source_node) {
        return delegate->mapped_name(source_node);
    }
} // mapper
