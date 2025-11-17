//
// Created by zhazha on 11/16/25.
//

#include "flacconversion_action.h"

#include <format>
#include <iostream>

#include "node.h"
#include "utils.h"

namespace mapper {
    flacconversion_action::flacconversion_action(dir_tree::node &source_node): source_node{source_node} {}

    int flacconversion_action::apply(dir_tree::node &target_node) {
        std::ostringstream oss;
        oss << "ffmpeg -loglevel quiet -nostats -hide_banner -y -i " << source_node.path <<
               " -c:v copy -c:a flac -map_metadata 0 ";
        if (!source_node.ctx.metadata_year.empty()) {
            oss << "-metadata year=" << source_node.ctx.metadata_year << " ";
        }

        oss << target_node.path;
        execution::run_cmd(oss.str());
        return 0;
    }

    std::string flacconversion_action::explain() {
        std::ostringstream ostringstream;
        ostringstream << "<Converting to flac from " << source_node.path << ">";
        return ostringstream.str();
    }
} // mapper