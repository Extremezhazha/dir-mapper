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
        map_orchestrator.cpp
        map_orchestrator.h
        preprocessors.h
        mapper.h
        map_action.h
        noop_action.cpp
        noop_action.h
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
)
