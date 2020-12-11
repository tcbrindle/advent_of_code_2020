
#include "../common.hpp"

#include <ranges>

constexpr auto sort = [](auto cont) {
    std::ranges::sort(cont);
    return cont;
};

// TODO: Add a proper lazy adjacent_difference to libflow (not this hack)
constexpr auto adjacent_difference = [](auto& in)
{
    return flow::slide(in, 2)
            .map([](auto win) { auto a = *win.next(); auto b = *win.next(); return b - a; });
};

constexpr  auto part1 = [](auto&& adaptors)
{
    const auto sorted = sort(adaptors);

    const auto [ones, threes] = adjacent_difference(sorted)
                                   .fold([](auto acc, auto diff) -> std::pair<int, int> {
                                       return {acc.first + (diff == 1), acc.second + (diff == 3)};
                                   }, std::pair{1, 1});

    return ones * threes;
};

const auto part2 = [](auto&& adaptors) -> int64_t
{

    auto vec = std::vector(adaptors.begin(), adaptors.end());
    // Sadly, I can't yet work out how to avoid having to stick this
    // zero onto the front of the sorted vector. If we could avoid it,
    // then this solution could be all nice and constexpr too.
    vec.push_back(0);
    std::ranges::sort(vec);

    auto cache = vec;

    return flow::ints(0, vec.size() - 1)
            .reverse()
            .fold([&](int64_t last, int64_t i) {
                cache[i] = last;
                return flow::ints(i+1)
                        .take(3)
                        .take_while([&](size_t j) {
                            return j < vec.size() && vec.at(j) - vec.at(i) <= 3; })
                        .map([&](auto j) { return cache.at(j-1); })
                        .sum();
            }, int64_t{1});
};

constexpr auto test_data1 = std::array<int64_t, 11>{
    16, 10, 15, 5, 1, 11, 7, 19, 6, 12, 4 };

constexpr auto test_data2 = std::array<int64_t, 31>{
    28, 33, 18, 42, 31, 14, 46, 20, 48, 47,
    24, 23, 49, 45, 19, 38, 39, 11, 1, 32,
    25, 35, 8, 17, 7, 9, 4, 2, 34, 10, 3 };

int main(int argc, char** argv)
{
    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    static_assert(part1(test_data1) == 35);
    static_assert(part1(test_data2) == 220);

    // No constexpr part 2 yet :(
    assert(part2(test_data1) == 8);
    assert(part2(test_data2) == 19208);

    const auto vec = aoc::vector_from_file<int64_t>(argv[1]);

    fmt::print("Part 1: {}\n", part1(vec));

    fmt::print("Part 2: {}\n", part2(vec));
}