#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <chrono>

#include "boost/process.hpp"

using namespace std::chrono_literals;

namespace bps = boost::process;

namespace example {
void start_child(std::string_view proc_name, const std::initializer_list<std::string>& args) {
    std::error_code ec;
    bps::ipstream in_pipe;
    bps::child child_process(bps::search_path(proc_name.data()), args, bps::std_out > in_pipe, ec);

    if (ec)
        std::cerr << ec.message() << std::endl;

    std::string line;
    while (child_process.running() && std::getline(in_pipe, line)) {
        std::cout << line << std::endl;
    }

    child_process.wait(ec);

    if (ec)
        std::cerr << ec.message() << std::endl;

    std::cout << "Returned value from child: " << child_process.exit_code() << std::endl;
}
} // namespace example
