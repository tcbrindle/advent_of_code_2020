
#include "../common.hpp"

#include "robin_hood.h"
#include <span>
#include <unordered_map>

const auto game = [](std::span<const int> input, int max) -> int
{
    auto seen = flow::copy(input).zip(flow::iota(1))
                     .to<robin_hood::unordered_map<int, int>>();

    return flow::ints(input.size(), max)
               .fold([&seen](int last, int idx) {
                    int x = std::exchange(seen[last], idx);
                    return x == 0 ? 0 : idx - x;
                }, input.back());
};

int main(int argc, char** argv)
{
    assert(game(std::array{0, 3, 6}, 2020) == 436);
    assert(game(std::array{1, 3, 2}, 2020) == 1);
    assert(game(std::array{2, 1, 3}, 2020) == 10);
    assert(game(std::array{1, 2, 3}, 2020) == 27);
    assert(game(std::array{2, 3, 1}, 2020) == 78);
    assert(game(std::array{3, 2, 1}, 2020) == 438);
    assert(game(std::array{3, 1, 2}, 2020) == 1836);


#ifdef TESTING
    assert(game(std::array{0, 3, 6}, 30'000'000) == 175594);
    assert(game(std::array{1, 3, 2}, 30'000'000) == 2578);
    assert(game(std::array{2, 1, 3}, 30'000'000) == 3544142);
    assert(game(std::array{1, 2, 3}, 30'000'000) == 261214);
    assert(game(std::array{2, 3, 1}, 30'000'000) == 6895259);
    assert(game(std::array{3, 2, 1}, 30'000'000) == 18);
    assert(game(std::array{3, 1, 2}, 30'000'000) == 362);
#endif

    // My personal input values:
    constexpr auto in = std::array{1,2,16,19,18,0};

    fmt::print("Part 1: {}\n", game(in, 2020));

    aoc::timer t;
    fmt::print("Part 2: {} ({})\n", game(std::array{1,2,16,19,18,0}, 30'000'000),
                t.elapsed<std::chrono::milliseconds>());
}