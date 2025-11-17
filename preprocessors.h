//
// Created by zhazha on 11/16/25.
//

#ifndef DIR_MAPPER_PREPROCESSORS_H
#define DIR_MAPPER_PREPROCESSORS_H
#include <iostream>
#include <regex>

#include "node.h"
#include "utils.h"

namespace preprocessors {
    inline int add_audio_type_to_m4a(dir_tree::node & node) {
        if (node.filetype == std::filesystem::file_type::regular &&
            node.path.filename().has_extension() &&
            node.path.filename().extension() == ".m4a") {
            std::ostringstream cmd;
            cmd << "ffprobe -v error -select_streams a:0 -show_entries stream=codec_name -of csv=p=0 " << node.path;
            std::string result = execution::run_cmd(cmd.str());

            const std::regex audio_type_regex{R"(^([a-z]+))"};

            std::smatch match;
            if (std::regex_search(result, match, audio_type_regex)) {
                node.ctx.audio_type = match[1];
            }
        }
        node.ctx.audited = true;
        return 0;
    }

    inline int add_year_to_alac(dir_tree::node & node) {
        if (node.ctx.audio_type == "alac") {
            std::ostringstream cmd;
            cmd << "ffprobe -v quiet -show_entries format_tags=date -of csv=p=0 " << node.path;
            std::string result = execution::run_cmd(cmd.str());

            const std::regex year_regex{R"(^([0-9]+))"};

            std::smatch match;
            if (std::regex_search(result, match, year_regex)) {
                node.ctx.metadata_year = match[1];
            }
        }
        return 0;
    }
}

#endif //DIR_MAPPER_PREPROCESSORS_H