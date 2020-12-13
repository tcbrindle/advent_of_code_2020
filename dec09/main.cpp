
#include "../common.hpp"

#include <span>

constexpr auto is_valid = [](int64_t target, auto window)
{
    // Is this a good way to do this? No. No it is not. At all.
    // I really need to write a cartesian_product adaptor...
    return window.subflow()
        .map([&window](int64_t i) {
            return flow::of(i)
                       .cycle()
                       .zip_with(std::plus<>{}, window.subflow());
        })
        .flatten()
        .contains(target);
};

constexpr auto part1 = [](std::span<int64_t const> input, int size) -> int64_t
{
    return flow::drop(input, size)
                .zip(flow::slide(input, size))
                .filter([](auto p) { return !is_valid(p.first, p.second); })
                .next().value().first;
};

constexpr auto part2 = [](std::span<int64_t const> input, int64_t target) -> int64_t
{
    return flow::ints(2, input.size())
              .map([&](auto sz) {
                  return flow::slide(input, sz)
                            .drop_while([&](auto win) { return win.sum() < target; })
                            .next().value();
              })
              .filter([&](auto win) { return win.sum() == target; })
              .map([](auto win) {
                  auto [min, max] = win.minmax().value();
                  return min + max;
              })
              .next().value();
};

constexpr int64_t test_data[] = {
35, 20, 15, 25, 47, 40, 62, 55, 65, 95, 102,
117, 150, 182, 127, 219, 299, 277, 309, 576};

int main(int argc, char** argv)
{
    static_assert(part1(test_data, 5) == 127);
    static_assert(part2(test_data, 127) == 62);

    if (argc < 2) {
        fmt::print(stderr, "Error, no input\n");
        return -1;
    }

    const auto vec = aoc::vector_from_file<int64_t>(argv[1]);

    const auto part1_answer = part1(vec, 25);

    fmt::print("Part 1: {}\n", part1_answer);

    fmt::print("Part 2: {}\n", part2(vec, part1_answer));
}