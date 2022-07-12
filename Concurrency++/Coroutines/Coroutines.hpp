#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <thread>

using namespace std::chrono_literals;

#include "cppcoro/generator.hpp"
#include "cppcoro/sync_wait.hpp"
#include "cppcoro/task.hpp"
#include "cppcoro/when_all_ready.hpp"

namespace example_1 {
cppcoro::generator<std::uint64_t> sum(std::uint64_t number) {
    std::uint64_t sum = 0;
    while (true) {
        co_yield sum += number;
    }
}

void start() {
    std::uint64_t number = 1;
    for (auto i : example_1::sum(number)) {
        std::cout << std::setw(2) << i << " ";

        if (i % 5 == 0 && i != 0) endl(std::cout);
        if (i == 20) break;

        std::this_thread::sleep_for(0.2s);
    }
}
}  // namespace example_1

namespace example_2 {
cppcoro::task<std::string> get_str() {
    constexpr const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint32_t> dist{10, static_cast<std::uint32_t>(std::ssize(alphanum) - 1)};
    auto rand = std::bind(dist, gen);

    std::string str;
    const std::uint32_t iterations = rand();
    str.reserve(iterations);
    for (std::size_t idx = 0; idx < iterations; idx++) str.push_back(alphanum[rand()]);

    co_return str;
}

cppcoro::task<> run() {
    constexpr std::size_t cap = 20;
    std::vector<cppcoro::task<std::string>> vec(cap);
    for (auto& task : vec) {
        task = get_str();
    }

    auto resultTasks = co_await cppcoro::when_all_ready(std::move(vec));

    for (auto& task : resultTasks) {
        try {
            std::cout << task.result() << "\n";
        } catch (const std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    co_return;
}

void start() { cppcoro::sync_wait(run()); }
}  // namespace example_2