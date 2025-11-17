//
// Created by zhazha on 11/15/25.
//

#ifndef DIR_MAPPER_UTILS_H
#define DIR_MAPPER_UTILS_H
#include <array>
#include <memory>
#include <string>

namespace execution {
    inline std::string run_cmd(std::string const & cmd) {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result.append(buffer.data());
        }
        return result;
    }
}
#endif //DIR_MAPPER_UTILS_H