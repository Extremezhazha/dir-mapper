cmake_minimum_required(VERSION 4.0)
project(dir_mapper)

set(CMAKE_CXX_STANDARD 20)

add_executable(dir_mapper main.cpp
        node.cpp
        node.h
        tree.cpp
        tree.h
        thread_pool.h
        utils.h
        mapper.h
        map_action.h
        createdir_action.cpp
        createdir_action.h
        copy_action.cpp
        copy_action.h
        cleanup_delegate_action.cpp
        cleanup_delegate_action.h
        flacconversion_action.cpp
        flacconversion_action.h
        dir_creation_mapper.cpp
        dir_creation_mapper.h
        file_copy_mapper.cpp
        file_copy_mapper.h
        refresh_mapper.cpp
        refresh_mapper.h
        alac_mapper.cpp
        alac_mapper.h
        libs/args/args.hxx
        pass.h
        pass_orchestrator.cpp
        pass_orchestrator.h
        init_tree_pass.cpp
        init_tree_pass.h
        source_m4a_audio_type_probe_pass.cpp
        source_m4a_audio_type_probe_pass.h
        alac_year_probe_pass.cpp
        alac_year_probe_pass.h
        file_mapping_pass.cpp
        file_mapping_pass.h
        dir_action_pass.cpp
        dir_action_pass.h
        file_action_pass.cpp
        file_action_pass.h
        hash_generating_pass.cpp
        hash_generating_pass.h
        cleanup_unaudited_pass.cpp
        cleanup_unaudited_pass.h
)
