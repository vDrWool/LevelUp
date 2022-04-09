#pragma once

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>



namespace sync {
namespace example_1 {
std::int32_t counter = 0;

void inc() {
    constexpr std::int32_t iters = 10000000;
    for (std::size_t idx = 0; idx < iters; idx++) {
        counter++;
    }
}

void start() {
    std::thread trd{&inc};

    inc();

    trd.join();

    std::cout << "[Example_1]Counter = " << counter << std::endl;
}
}  // namespace example_1

namespace example_2 {
std::int32_t counter = 0;
std::mutex mutex;

void inc() {
    constexpr std::int32_t iters = 10000000;
    for (std::size_t idx = 0; idx < iters; idx++) {
        std::scoped_lock lock(mutex);
        counter++;
    }
}

void start() {
    std::thread trd{&inc};

    inc();

    trd.join();

    std::cout << "[Example_1]Counter = " << counter << std::endl;
}
}  // namespace example_2

namespace example_3 {
std::atomic<std::int32_t> counter = 0;

void inc() {
    constexpr std::int32_t iters = 10000000;
    for (std::size_t idx = 0; idx < iters; idx++) {
        counter++;
    }
}

void start() {
    std::thread trd{&inc};

    inc();

    trd.join();

    std::cout << "[Example_1]Counter = " << counter << std::endl;
}
}  // namespace example_3

namespace example_4 {
std::atomic<std::int32_t> counter = 0;

void inc(std::int32_t arg) {
    constexpr std::int32_t iters = 10000000;
    for (std::size_t idx = 0; idx < iters; idx++) {
        counter.fetch_add(arg);
    }
}

void start() {
    std::thread trd{&inc, 5};

    inc(5);

    trd.join();

    std::cout << "[Example_1]Counter = " << counter << std::endl;
}
}  // namespace example_4
}  // namespace sync