#include <iostream>
#include <regex>

#include "source_m4a_audio_type_probe_pass.h"
#include "utils.h"
#include "thread_pool.h"

namespace {
    void probe_m4a_file(dir_tree::node &node) {
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
    }
}

namespace mapper::passes {
    source_m4a_audio_type_probe_pass::source_m4a_audio_type_probe_pass(bool enable_parallel,
                                                                       int thread_pool_size) : enable_parallel{
            enable_parallel
        },
        thread_pool_size{thread_pool_size} {
    }

    int source_m4a_audio_type_probe_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        if (enable_parallel) {
            execution::thread_pool<int> preprocessing_pool{thread_pool_size};
            source.visit_all([&preprocessing_pool](dir_tree::node &node) {
                preprocessing_pool.submit([&node] {
                    probe_m4a_file(node);
                    return 0;
                });
            }, false);
        } else {
            source.visit_all(probe_m4a_file, false);
        }
        if (debug) {
            std::cout << "=================== Probed Source tree ===================" << std::endl;
            source.print();
        }
        return 0;
    }
}
