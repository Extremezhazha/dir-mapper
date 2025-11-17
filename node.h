#ifndef DIR_MAPPER_NODE_H
#define DIR_MAPPER_NODE_H

#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>
#include <map>
#include <unordered_set>

#include "map_action.h"

namespace dir_tree {
    struct node_process_ctx {
        bool audited;
        std::string audio_type;
        std::string metadata_year;
        std::unique_ptr<mapper::mapper_action> action;
        std::unordered_set<std::string> recursive_print_tags;
        node_process_ctx(): audited{false} {}
        [[nodiscard]] std::string get_str() const;
    };

    struct node {
    private:
        using m_map_t = std::map<std::string, node>;
        using timestamp_t = std::chrono::time_point<std::chrono::file_clock>;
        void print(std::string const & skipping, bool is_root, bool is_last, std::string const &print_tag) const;
        [[nodiscard]] std::string get_md() const;
    public:
        bool exists;
        std::filesystem::file_type filetype;
        m_map_t m_map;
        timestamp_t m_time;
        std::filesystem::path path;
        node_process_ctx ctx;
        explicit node(std::filesystem::path const & p);
        explicit node(std::filesystem::path const & p, bool skip_probe);
        node* get_next(std::string const & name);
        void init();
        void print() const;
        void print(std::string const & tag) const;
        void visit(const std::function<void(node&)>& visitor);
        void visit_all(const std::function<void(node&)>& visitor, bool postfix);
        void eval_recursive_print_tag(const std::string & tag, const std::function<bool(node&)>& decider);
    };
} // dir_tree

#endif //DIR_MAPPER_NODE_H