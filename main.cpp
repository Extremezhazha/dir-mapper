#include "alac_mapper.h"
#include "dir_creation_mapper.h"
#include "file_copy_mapper.h"
#include "map_orchestrator.h"
#include "preprocessors.h"
#include "refresh_mapper.h"
#include "libs/args/args.hxx"

namespace {
    int process(int thread_count, bool enable_parallel, std::string const & input_path_str, std::string const & output_path_str, bool is_dryrun) {
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


        orchestration::map_orchestrator::preprocessor_vec_t preprocessors{
            preprocessors::add_audio_type_to_m4a,
            preprocessors::add_year_to_alac
        };

        orchestration::map_orchestrator::mapper_vec_t mappers;

        mappers.emplace_back(std::make_unique<mapper::dir_creation_mapper>());
        mappers.emplace_back(std::make_unique<mapper::refresh_mapper>(std::make_unique<mapper::alac_mapper>()));
        mappers.emplace_back(std::make_unique<mapper::refresh_mapper>(std::make_unique<mapper::file_copy_mapper>()));

        orchestration::map_orchestrator orchestrator{
            thread_count,
            enable_parallel,
            std::move(preprocessors),
            std::move(mappers),
            input_path,
            output_path
        };

        orchestrator.init(false);
        orchestrator.create_mapping(true);
        if (!is_dryrun) {
            orchestrator.do_work();
        }
        return 0;
    }
}

int main(int argc, char **argv) {
    args::ArgumentParser parser("Handles checking and converting + copying files in parallel. ", "");
    args::Group arguments(parser, "arguments", args::Group::Validators::DontCare, args::Options::Global);

    args::Flag dryrun(arguments, "dryrun", "The druryn flag", {"dryrun"});

    args::ValueFlag<std::string> input_path_str(
    arguments, "input-path", "Input path", {"input-path"});
    args::ValueFlag<std::string> output_path_str(
    arguments, "output-path", "Output path", {"output-path"});
    args::ValueFlag<int> thread_count(
        arguments, "thread-count", "Number of threads for time consuming actions", {'t', "thread-count"});
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e)
    {
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
        thread_count_parsed > 0 ,
        input_path_str.Get(),
        output_path_str.Get(),
        dryrun
    );
}
