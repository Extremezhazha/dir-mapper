#include "cleanup_unaudited_pass.h"

#include <iostream>
#include <set>

namespace mapper::passes {
    int cleanup_unaudited_pass::run(dir_tree::node &source, dir_tree::node &target, bool debug) {
        if (debug) {
            auto [dirs, files] = target.visit_all<std::pair<int, int> >(
                [](dir_tree::node &node) -> std::pair<int, int> {
                    if (!node.ctx.audited) {
                        if (node.filetype == std::filesystem::file_type::directory) {
                            return {1, 0};
                        }
                        return {0, 1};
                    }
                    return {0, 0};
                },
                false,
                [](std::pair<int, int> base, std::vector<std::pair<int, int> > childs) -> std::pair<int, int> {
                    auto [dirs, files] = base;
                    for (auto [dir, file]: childs) {
                        dirs += dir;
                        files += file;
                    }
                    return {dirs, files};
                }
            );

            std::cout << "Will delete " << dirs << " directories and " << files << " files." << std::endl;
        }

        target.visit_all([debug](dir_tree::node &node) {
            std::set<std::string> to_delete;
            for (auto &[key, child]: node.m_map) {
                if (!child.ctx.audited) {
                    to_delete.emplace(key);
                }
            }

            for (auto &key: to_delete) {
                node.m_map.erase(key);
            }

            if (!node.ctx.audited && node.m_map.empty()) {
                if (debug) {
                    std::cout << "Deleting " << node.path << std::endl;
                }
                std::filesystem::remove(node.path);
            }
        }, true);
        return 0;
    }
}
