#include "alac_mapper.h"
#include "alac_year_probe_pass.h"
#include "cleanup_unaudited_pass.h"
#include "dir_action_pass.h"
#include "dir_creation_mapper.h"
#include "file_action_pass.h"
#include "file_copy_mapper.h"
#include "file_mapping_pass.h"
#include "hash_generating_pass.h"
#include "init_tree_pass.h"
#include "pass_orchestrator.h"
#include "refresh_mapper.h"
#include "source_m4a_audio_type_probe_pass.h"
#include "libs/args/args.hxx"

namespace {
    int process(
        int thread_count,
        bool enable_parallel,
        std::string const &input_path_str,
        std::string const &output_path_str,
        bool enable_alac_conversion,
        bool handle_alac_year,
        bool enable_hash_check,
        bool enable_cleanup,
        bool is_dryrun,
        bool debug
    ) {
        std::filesystem::path input_path{input_path_str};
        std::filesystem::path output_path{output_path_str};
        if (!std::filesystem::exists(input_path) || !std::filesystem::is_directory(input_path)) {
            std::cerr << "Input path does not exist" << std::endl;
            return 1;
        }
        if (!std::filesystem::exists(output_path)) {
            std::filesystem::create_directory(output_path);
        }
        if (!std::filesystem::is_directory(output_path)) {
            std::cerr << "Output path is not a directory" << std::endl;
            return 1;
        }

        std::vector<std::unique_ptr<mapper::pass> > passes;
        passes.emplace_back(std::make_unique<mapper::passes::init_tree_pass>());
        if (enable_alac_conversion) {
            passes.emplace_back(
                std::make_unique<mapper::passes::source_m4a_audio_type_probe_pass>(enable_parallel, thread_count));
            if (handle_alac_year) {
                passes.emplace_back(
                    std::make_unique<mapper::passes::alac_year_probe_pass>(enable_parallel, thread_count));
            }
        }
        if (enable_hash_check) {
            passes.emplace_back(std::make_unique<mapper::passes::hash_generating_pass>(enable_parallel, thread_count));
        }

        mapper::passes::file_mapping_pass::mapper_vec_t mappers;

        mappers.emplace_back(std::make_unique<mapper::dir_creation_mapper>());
        if (enable_alac_conversion) {
            mappers.emplace_back(
                std::make_unique<mapper::refresh_mapper>(std::make_unique<mapper::alac_mapper>(), false));
        }
        mappers.emplace_back(
            std::make_unique<mapper::refresh_mapper>(std::make_unique<mapper::file_copy_mapper>(), enable_hash_check));

        passes.emplace_back(
            std::make_unique<mapper::passes::file_mapping_pass>(enable_parallel, thread_count, std::move(mappers)));

        if (!is_dryrun) {
            passes.emplace_back(std::make_unique<mapper::passes::dir_action_pass>());
            passes.emplace_back(std::make_unique<mapper::passes::file_action_pass>(enable_parallel, thread_count));
            if (enable_cleanup) {
                passes.emplace_back(std::make_unique<mapper::passes::cleanup_unaudited_pass>());
            }
        }

        mapper::pass_orchestrator pass_orchestrator{input_path, output_path, std::move(passes)};

        return pass_orchestrator.run(debug);
    }
}

int main(int argc, char **argv) {
    args::ArgumentParser parser("Handles checking and converting + copying files in parallel. ", "");
    args::Group arguments(parser, "arguments", args::Group::Validators::DontCare, args::Options::Global);

    args::Flag enable_alac_conversion(arguments, "enable-alac-conversion", "Enables alac conversion",
                                      {"enable-alac-conversion"});

    args::Flag handle_alac_year(arguments, "handle-alac-year", "Handles alac year metadata", {"handle-alac-year"});

    args::Flag enable_hash_check(arguments, "enable-hash-check", "Checks the hash when deciding to copy file over",
                                 {"enable-hash-check"});

    args::Flag enable_cleanup(arguments, "cleanup", "The cleanup flag", {"cleanup"});

    args::Flag dryrun(arguments, "dryrun", "The druryn flag", {"dryrun"});

    args::Flag debug(arguments, "debug", "The debug flag", {"debug"});

    args::ValueFlag<std::string> input_path_str(
        arguments, "input-path", "Input path", {"input-path"});
    args::ValueFlag<std::string> output_path_str(
        arguments, "output-path", "Output path", {"output-path"});
    args::ValueFlag<int> thread_count(
        arguments, "thread-count", "Number of threads for time consuming actions", {'t', "thread-count"});
    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    if (!input_path_str || !output_path_str) {
        std::cout << "Missing required fields" << std::endl;
        std::cout << parser;
        return 0;
    }
    int thread_count_parsed = 0;
    if (thread_count)
        thread_count_parsed = thread_count.Get();
    return process(
        thread_count_parsed,
        thread_count_parsed > 0,
        input_path_str.Get(),
        output_path_str.Get(),
        enable_alac_conversion,
        handle_alac_year,
        enable_hash_check,
        enable_cleanup,
        dryrun,
        debug
    );
}
