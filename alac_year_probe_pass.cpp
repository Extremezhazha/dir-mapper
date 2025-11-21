#include <iostream>
#include <regex>

#include "alac_year_probe_pass.h"
#include "utils.h"
#include "thread_pool.h"

namespace {
    void probe_alac_file_year(dir_tree::node &node) {
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
    }
}

namespace mapper::passes {
    alac_year_probe_pass::alac_year_probe_pass(bool enable_parallel, int thread_pool_size) : enable_parallel{
            enable_parallel
        },
        thread_pool_size{thread_pool_size} {
    }

    int alac_year_probe_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        if (enable_parallel) {
            execution::thread_pool<int> preprocessing_pool{thread_pool_size};
            source.visit_all([&preprocessing_pool](dir_tree::node &node) {
                preprocessing_pool.submit([&node] {
                    probe_alac_file_year(node);
                    return 0;
                });
            }, false);
        } else {
            source.visit_all(probe_alac_file_year, false);
        }
        if (debug) {
            std::cout << "=================== Probed Source tree with year ===================" << std::endl;
            source.print();
        }
        return 0;
    }
}
