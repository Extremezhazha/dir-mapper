//
// Created by zhazha on 11/15/25.
//

#include "node.h"

#include <iostream>
#include <ranges>

namespace dir_tree {
        std::string node_process_ctx::get_str() const {
                std::string result = this->audio_type;
                if (this->audited) {
                        result += " audited";
                }
                if (this->action) {
                        result += " " + this->action->explain();
                }
                return result;
        }

        void node::print(std::string const & skipping, bool is_root, bool is_last, std::string const &print_tag) const {
                if (!print_tag.empty() && !ctx.recursive_print_tags.contains(print_tag)) {
                        if (is_root)
                                std::cout << "No actions!" << std::endl;
                        return;
                }
                if (!is_root) {
                        std::cout << skipping << (is_last ? "└" : "├") << "── " << this->path.filename();
                } else {
                        std::cout << this->path;
                }
                std::cout << " " << this->get_md() << std::endl;
                std::string child_skipping = skipping;
                if (!is_root) {
                        child_skipping += is_last ? " " : "│";
                        child_skipping += "   ";
                }
                std::string const & child_skipping_ref = child_skipping;
                int i = 0;
                if (print_tag.empty()) {
                        for (const auto &v: m_map | std::views::values) {
                                v.print(child_skipping_ref, false, ++i == m_map.size(), print_tag);
                        }
                } else {
                        std::vector<std::reference_wrapper<node const>> tagged_children;
                        for (const auto &v: m_map | std::views::values) {
                                if (v.ctx.recursive_print_tags.contains(print_tag)) {
                                        tagged_children.emplace_back(v);
                                }
                        }
                        for (const auto &v: tagged_children) {
                                v.get().print(child_skipping_ref, false, ++i == tagged_children.size(), print_tag);
                        }
                }
        }

        std::string node::get_md() const {
                std::string result;
                switch (filetype) {
                        case std::filesystem::file_type::directory:
                                result += "Dir";
                                break;
                        case std::filesystem::file_type::regular:
                                result += "File";
                                break;
                        case std::filesystem::file_type::none:
                                result += "Null";
                                break;
                        default:
                                result += "Unknown";
                }

                std::string ctx_str = ctx.get_str();
                if (ctx_str != "")
                        result += " " + ctx_str;

                if (exists) {
                        result += " " + std::format("{:%F %T}", std::chrono::floor<std::chrono::seconds>(
                                                std::chrono::file_clock::to_sys(m_time)));
                }
                return result;
        }

        node::node(const std::filesystem::path &p): node(p, false) {}

        node::node(std::filesystem::path const &p, bool skip_probe):
        path{p}, filetype{std::filesystem::file_type::none}, exists{false} {
                if (!skip_probe)
                        this->init();
        }

        node * node::get_next(std::string const &name) {
                auto found = this->m_map.find(name);
                return found == this->m_map.end() ? nullptr : &found->second;
        }

        void node::init() {
                exists = std::filesystem::exists(path);
                if (exists) {
                        filetype = std::filesystem::status(path).type();
                        m_time = std::filesystem::last_write_time(path);
                }
        }

        void node::print() const {
                print({});
        }

        void node::print(std::string const & tag) const {
                print("", true, false, tag);
        }

        void node::visit(const std::function<void(node &)>& visitor) {
                visitor(*this);
        }

        void node::visit_all(const std::function<void(node &)>& visitor, const bool postfix) {
                if (!postfix) {
                        this->visit(visitor);
                }
                for (auto &v: this->m_map | std::views::values) {
                        v.visit_all(visitor, postfix);
                }
                if (postfix) {
                        this->visit(visitor);
                }
        }

        void node::eval_recursive_print_tag(const std::string &tag, const std::function<bool(node &)>& decider) {
                bool apply_tag = decider(*this);
                for (auto &v: m_map | std::views::values) {
                        v.eval_recursive_print_tag(tag, decider);
                        apply_tag = apply_tag || v.ctx.recursive_print_tags.contains(tag);
                }
                if (apply_tag)
                        this->ctx.recursive_print_tags.emplace(tag);
        }
} // dir_tree