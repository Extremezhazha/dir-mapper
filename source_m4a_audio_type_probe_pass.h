#ifndef DIR_MAPPER_AUDIO_TYPE_PROBE_PASS_H
#define DIR_MAPPER_AUDIO_TYPE_PROBE_PASS_H
#include "pass.h"

namespace mapper::passes {
    class source_m4a_audio_type_probe_pass : public pass {
    public:
        source_m4a_audio_type_probe_pass(bool enable_parallel, int thread_pool_size);

    private:
        bool enable_parallel;
        int thread_pool_size;

    public:
        int run(dir_tree::node &source, dir_tree::node &target, bool debug) override;
    };
}

#endif //DIR_MAPPER_AUDIO_TYPE_PROBE_PASS_H
