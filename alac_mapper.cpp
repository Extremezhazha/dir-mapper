#include "alac_mapper.h"

#include "flacconversion_action.h"

namespace mapper {
    alac_mapper::~alac_mapper() = default;

    mapper::result_t alac_mapper::apply(dir_tree::node &source_node, dir_tree::node *target_node) {
        return std::make_pair(true, std::make_unique<flacconversion_action>(source_node));
    }

    bool alac_mapper::should_try_apply(dir_tree::node &source_node) {
        return source_node.exists && source_node.ctx.audio_type == "alac";
    }

    std::string alac_mapper::mapped_name(dir_tree::node &source_node) {
        return source_node.path.filename().replace_extension(".flac");
    }
} // mapper
